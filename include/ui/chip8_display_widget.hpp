#ifndef CHIP8_UI_CHIP8_DISPLAY_WIDGET_HPP
#define CHIP8_UI_CHIP8_DISPLAY_WIDGET_HPP

#include <QColor>
#include <QWidget>
#include <array>
#include <cstdint>

namespace chip8 {
namespace ui {

/**
 * @brief Renders the CHIP-8 monochrome framebuffer.
 *
 * The widget scales the fixed 64x32 framebuffer to its current size and lets
 * the foreground/background colours and an optional pixel grid be customised.
 */
class DisplayWidget : public QWidget {
    Q_OBJECT

public:
    /// Native framebuffer width in pixels.
    static constexpr int kDisplayWidth = 64;
    /// Native framebuffer height in pixels.
    static constexpr int kDisplayHeight = 32;

    /**
     * @brief Constructs the widget with default colours.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit DisplayWidget(QWidget* parent = nullptr);

    /**
     * @brief Replaces the displayed framebuffer and schedules a repaint.
     * @param framebuffer Row-major array of 64x32 pixels (0 = off, 1 = on).
     */
    void update_display(const std::array<std::uint8_t, kDisplayWidth * kDisplayHeight>& framebuffer);

    /**
     * @brief Sets the colour used to draw lit pixels.
     * @param color Foreground colour.
     */
    void set_pixel_color(const QColor& color);

    /**
     * @brief Sets the colour used to fill the background.
     * @param color Background colour.
     */
    void set_background_color(const QColor& color);

    /**
     * @brief Enables or disables the pixel grid overlay.
     * @param show_grid @c true to draw grid lines between pixels.
     */
    void set_show_grid(bool show_grid);

protected:
    /**
     * @brief Paints the framebuffer scaled to the widget size.
     * @param event Paint event (unused).
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Returns the preferred widget size.
     * @return Default size at a 10x scale factor.
     */
    [[nodiscard]] QSize sizeHint() const override;

    /**
     * @brief Returns the smallest acceptable widget size.
     * @return Minimum size at a 2x scale factor.
     */
    [[nodiscard]] QSize minimumSizeHint() const override;

private:
    std::array<std::uint8_t, kDisplayWidth * kDisplayHeight> framebuffer_{};
    QColor pixel_color_;
    QColor background_color_;
    bool show_grid_;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_DISPLAY_WIDGET_HPP
