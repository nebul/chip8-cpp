#include "ui/chip8_display_widget.hpp"

#include <QPainter>

namespace chip8 {
namespace ui {
namespace {

/// Pixel size, in screen pixels, used to derive the default widget size.
constexpr int kDefaultScale = 10;
/// Pixel size used to derive the minimum widget size.
constexpr int kMinimumScale = 2;

} // namespace

DisplayWidget::DisplayWidget(QWidget* parent)
    : QWidget(parent)
    , pixel_color_(Qt::white)
    , background_color_(Qt::black)
    , show_grid_(false) {
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(kDisplayWidth * kMinimumScale, kDisplayHeight * kMinimumScale);
}

void DisplayWidget::update_display(
    const std::array<std::uint8_t, kDisplayWidth * kDisplayHeight>& framebuffer) {
    framebuffer_ = framebuffer;
    update();
}

void DisplayWidget::set_pixel_color(const QColor& color) {
    pixel_color_ = color;
    update();
}

void DisplayWidget::set_background_color(const QColor& color) {
    background_color_ = color;
    update();
}

void DisplayWidget::set_show_grid(bool show_grid) {
    show_grid_ = show_grid;
    update();
}

void DisplayWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    const qreal pixel_width = static_cast<qreal>(width()) / kDisplayWidth;
    const qreal pixel_height = static_cast<qreal>(height()) / kDisplayHeight;

    painter.fillRect(rect(), background_color_);

    painter.setBrush(pixel_color_);
    painter.setPen(Qt::NoPen);
    for (int y = 0; y < kDisplayHeight; ++y) {
        for (int x = 0; x < kDisplayWidth; ++x) {
            if (framebuffer_[y * kDisplayWidth + x] != 0) {
                painter.drawRect(QRectF(x * pixel_width, y * pixel_height,
                                        pixel_width, pixel_height));
            }
        }
    }

    if (show_grid_) {
        painter.setPen(QPen(QColor(50, 50, 50, 128), 1));
        for (int y = 0; y <= kDisplayHeight; ++y) {
            painter.drawLine(QPointF(0, y * pixel_height),
                             QPointF(width(), y * pixel_height));
        }
        for (int x = 0; x <= kDisplayWidth; ++x) {
            painter.drawLine(QPointF(x * pixel_width, 0),
                             QPointF(x * pixel_width, height()));
        }
    }
}

QSize DisplayWidget::sizeHint() const {
    return QSize(kDisplayWidth * kDefaultScale, kDisplayHeight * kDefaultScale);
}

QSize DisplayWidget::minimumSizeHint() const {
    return QSize(kDisplayWidth * kMinimumScale, kDisplayHeight * kMinimumScale);
}

} // namespace ui
} // namespace chip8
