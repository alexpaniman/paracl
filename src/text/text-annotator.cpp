#include "paracl/text/text-annotator.h"
#include "paracl/text/colored-text.h"
#include "paracl/text/display.h"


namespace paracl {

namespace {

class text_annotator {
public:
    text_annotator(annotation_config config,
                   std::span<char> text,
                   std::span<annotated_range> ranges):
        config_(config),
        text_(text),
        ranges_(ranges) {


        assert(!ranges_.empty());
        std::sort(ranges_.begin(), ranges_.end());
    }

    const colored_text& emit() {
        auto &position_open = ranges_[0].range.begin;
        auto &position_stop = ranges_[ranges_.size() - 1].range.end;

        // compute the beginning line:
        int32_t i = position_open.point - position_open.column;

        int32_t line_min = position_open.line;
        int32_t line_max = get_previous_iter_line(position_stop);

        size_t annotation_column = get_annotation_column();

        for (int32_t line = line_min; line <= line_max; ++ line) {
            print_line_number(line);

            int32_t start = i;
            print_line(i);

            int32_t end = i;

            bool has_annotations =
                print_annotations(start, end, /*dry_run=*/true);

            if (!has_annotations)
                continue;

            print_line_number("");
            print_annotations(start, end, /*dry_run=*/false);

            auto line_ranges = get_join_points(line);
            if (!line_ranges.empty())
                print_messages(std::move(line_ranges), annotation_column);
        }

        return annotated_text_;
    }

private:
    annotation_config config_;

    std::span<char> text_;
    std::span<annotated_range> ranges_;

    colored_text annotated_text_;

    void print_line(int32_t &i) {
        int32_t size = text_.size();

        for (; i < size && text_[i] != '\n'; ++ i)
            annotated_text_.append("{}", text_[i]);

        ++ i;
        annotated_text_.append("\n");
    }

    bool print_annotations(size_t start, size_t end, bool dry_run = false) {
        bool has_annotations = false;
        for (size_t i = start; i < end; ++ i) {
            bool beginning = false;
            bool inside = false;

            bool should_connect = false;

            for (size_t j = 0; j < ranges_.size(); ++ j) {
                if (i <  ranges_[j].range.begin.point)
                    continue;

                if (i == ranges_[j].range.begin.point) {
                    inside = true;
                    beginning = true;

                    should_connect |= !ranges_[j].annotation.empty();
                    continue;
                }

                if (i <  ranges_[j].range.end.point)
                    inside = true;
            }

            has_annotations |= inside;

            if (dry_run)
                continue;

            if (!inside)
                annotated_text_.append(" ");
            else {
                if (beginning && should_connect)
                    append_arrow_part(config_.arrow_config.underline_connection);
                else {
                    append_arrow_part(config_.arrow_config.underline);
                }
            }
        }

        if (!dry_run)
            annotated_text_.append("\n");

        return has_annotations;
    }

    void print_line_number(auto line) {
        annotated_text_.append("{:>{}} | ", line, config_.line_numbers_space);
    }

    int32_t get_previous_iter_line(text_position pos) {
        return pos.line - (pos.column == 0 ? 1 : 0);
    }

    struct annotated_point {
        text_position position;
        std::string annotation;

        auto operator<=>(const annotated_point &other) const {
            return position <=> other.position;
        }
    };

    void print_messages(std::vector<annotated_point> ranges, size_t column_alignment = 0) {
        std::sort(ranges.begin(), ranges.end());

        size_t max_column = column_alignment;
        for (int j = ranges.size() - 1; j >= 0; -- j) {
            print_line_number("");

            size_t current_column = 0;
            for (int i = 0; i <= j; ++ i) {
                auto &[pos, annotation] = ranges[i];

                while (current_column < pos.column) {
                    annotated_text_.append(" ");
                    ++ current_column;
                }

                if (i == j) {
                    if (current_column > max_column)
                        max_column = current_column;

                    if (current_column < max_column) {
                        append_arrow_part(config_.arrow_config.leftwards_upwards_join);
                        ++ current_column;
                    }

                    while (current_column < max_column) {
                        append_arrow_part(config_.arrow_config.leftwards_line);
                        ++ current_column;
                    }

                    annotated_text_.append(" {}", annotation);

                    current_column += annotation.size() + 1;
                } else {
                    append_arrow_part(config_.arrow_config.upwards_line);
                    ++ current_column;
                }
            }

            annotated_text_.append("\n");
        }

        print_line_number("");
        annotated_text_.append("\n");
    }

    std::vector<annotated_point> get_join_points(size_t line) {
        std::vector<annotated_point> line_ranges;
        for (const auto &annotated_range: ranges_) {
            if (annotated_range.range.begin.line != line)
                continue;

            if (annotated_range.annotation.empty())
                continue;

            line_ranges.push_back({
                annotated_range.range.begin,
                annotated_range.annotation
            });
        }

        return line_ranges;
    }

    size_t get_annotation_column() {
        size_t max_column = 0;
        for (auto &&range: ranges_) {
            if (max_column < range.range.begin.column)
                max_column = range.range.begin.column;
        }

        max_column += 2;

        return max_column;
    }

    void append_arrow_part(const char *string) {
        annotated_text_.set_formatting(config_.line_format);
        annotated_text_.append("{}", string);
        annotated_text_.clear_formatting();
    }
};

} // end anonymous namespace

colored_text annotate(std::span<char> text, std::vector<annotated_range> ranges,
                      annotation_config config) {

    text_annotator annotator{config, text, ranges};
    return annotator.emit(); // TODO: move
}

} // end namespace paracl
