#!/usr/bin/env python3
"""Replicate TaiJiView::DrawObject math to produce preview.gif / preview.png."""
import math
from pathlib import Path

from PIL import Image, ImageDraw

PI = math.pi
SIZE = 480
CX = CY = SIZE // 2
# Scale so diameter ~360 fits in 480 with margin (original R1=320)
R1 = 180
R2 = R1 // 2
R3 = R1 // 8
BG = (32, 32, 36)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)


def round_i(d: float) -> int:
    return int(math.floor(d + 0.5))


def to_img(x: float, y: float) -> tuple[int, int]:
    """Logical (Y-up, origin center) -> image (Y-down, origin top-left)."""
    return round_i(CX + x), round_i(CY - y)


def draw_taiji(draw: ImageDraw.ImageDraw, i: int) -> None:
    """Match fixed DrawObject: pies + filled heads/eyes; angle i in degrees."""
    # Arc endpoints on outer circle
    x1 = R1 * math.cos((i + 90) * PI / 180)
    y1 = R1 * math.sin((i + 90) * PI / 180)
    x2 = R1 * math.cos((i - 90) * PI / 180)
    y2 = R1 * math.sin((i - 90) * PI / 180)

    # Bounding box of outer circle in image coords
    bbox = [CX - R1, CY - R1, CX + R1, CY + R1]

    # Pillow pieslice: 0° = 3 o'clock, increases clockwise, Y-down.
    # Math angle θ (CCW from +X, Y-up) -> pillow start = -θ.
    # Black half: MFC Pie from (i-90) to (i+90) CCW = right half when i=0.
    # In pillow, sweep the same logical CCW arc by going from -a_start to -a_end
    # with end > start in clockwise pillow space... easier: fill two 180° slices.
    a_top = i + 90   # math degrees of point3
    a_bot = i - 90   # math degrees of point4

    def math_to_pillow(a_deg: float) -> float:
        return (-a_deg) % 360

    # Black: CCW from a_bot to a_top in math = pillow clockwise from math_to_pillow(a_bot)
    # to math_to_pillow(a_top). Pillow pieslice goes clockwise from start to end.
    # Math CCW from A to B equals pillow clockwise from -A to -B.
    start_b = math_to_pillow(a_bot)
    end_b = math_to_pillow(a_top)
    # Ensure clockwise sweep of 180°
    if (end_b - start_b) % 360 != 180:
        # normalize to 180° sweep clockwise
        end_b = (start_b + 180) % 360

    draw.pieslice(bbox, start=start_b, end=(start_b + 180) % 360 if False else start_b + 180, fill=BLACK)
    # White other half
    draw.pieslice(bbox, start=start_b + 180, end=start_b + 360, fill=WHITE)

    # Black fish head at (i-90)
    cx_b = R2 * math.cos((i - 90) * PI / 180)
    cy_b = R2 * math.sin((i - 90) * PI / 180)
    ix, iy = to_img(cx_b, cy_b)
    draw.ellipse([ix - R2, iy - R2, ix + R2, iy + R2], fill=BLACK)

    # White fish head at (i+90)
    cx_w = R2 * math.cos((i + 90) * PI / 180)
    cy_w = R2 * math.sin((i + 90) * PI / 180)
    ix, iy = to_img(cx_w, cy_w)
    draw.ellipse([ix - R2, iy - R2, ix + R2, iy + R2], fill=WHITE)

    # White eye on black head
    ix, iy = to_img(cx_b, cy_b)
    draw.ellipse([ix - R3, iy - R3, ix + R3, iy + R3], fill=WHITE)

    # Black eye on white head
    ix, iy = to_img(cx_w, cy_w)
    draw.ellipse([ix - R3, iy - R3, ix + R3, iy + R3], fill=BLACK)

    # Outer ring outline for clarity
    draw.ellipse(bbox, outline=(200, 200, 200), width=1)


def render_frame(angle: int) -> Image.Image:
    img = Image.new("RGB", (SIZE, SIZE), BG)
    draw = ImageDraw.Draw(img)
    draw_taiji(draw, angle)
    return img


def main() -> None:
    out = Path(__file__).resolve().parent
    frames = []
    # Match C++: DrawObject(pDC, -m_angle) while m_angle goes 0,2,4,...
    for step in range(36):
        m_angle = step * 10  # 36 frames * 10° = full turn
        frames.append(render_frame(-m_angle))

    still = render_frame(-45)
    still_path = out / "preview.png"
    still.save(still_path, "PNG")
    print(f"wrote {still_path}")

    gif_path = out / "preview.gif"
    frames[0].save(
        gif_path,
        save_all=True,
        append_images=frames[1:],
        duration=50,
        loop=0,
        optimize=False,
    )
    print(f"wrote {gif_path} ({len(frames)} frames)")


if __name__ == "__main__":
    main()
