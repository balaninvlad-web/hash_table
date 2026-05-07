import os
from PIL import Image, ImageDraw, ImageFont

def parse_distribution(filepath):
    """Возвращает список counts длиной 4096 (0..4095)"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read().strip()
    # Переводим в список чисел
    content = content.replace('\n', ',')
    parts = [p.strip() for p in content.split(',') if p and p.strip()]
    if parts and parts[0].lower() in ('bucket_index', 'word_count'):
        parts = parts[2:]
    numbers = []
    for p in parts:
        try:
            numbers.append(float(p))
        except:
            continue
    if len(numbers) % 2 != 0:
        numbers = numbers[:-1]
    indices = [int(i) for i in numbers[0::2]]
    counts = numbers[1::2]
    # Создаём массив на 4096 элементов, заполняем нулями
    full_counts = [0] * 4096
    for idx, cnt in zip(indices, counts):
        if 0 <= idx < 4096:
            full_counts[idx] = cnt
    return full_counts

def draw_histogram_full(counts, title, out_path, width=4096, height=800):
    """
    Рисует гистограмму для всех 4096 корзин.
    width = 4096 пикселей -> один столбец = 1 пиксель (без промежутков)
    height = 800 пикселей
    """
    margin_left = 80     # для подписей оси Y
    margin_right = 40
    margin_top = 60      # для заголовка
    margin_bottom = 60   # для подписей оси X

    # Область рисования столбцов
    chart_width = width - margin_left - margin_right
    chart_height = height - margin_top - margin_bottom
    # Ширина одного столбца = 1 пиксель (без зазора)
    bar_width = 1
    # Количество корзин = 4096
    num_bins = len(counts)
    # Масштаб по Y
    max_count = max(counts) if counts else 1
    y_scale = chart_height / max_count if max_count > 0 else 1

    # Создаём изображение
    img = Image.new('RGB', (width, height), color='white')
    draw = ImageDraw.Draw(img)

    # Рисуем рамку для области графика
    draw.rectangle([margin_left, margin_top,
                    margin_left + chart_width, margin_top + chart_height],
                   outline='black', width=1)

    # Рисуем столбцы
    for i in range(num_bins):
        cnt = counts[i]
        if cnt == 0:
            continue
        # Высота столбца
        bar_height = int(cnt * y_scale)
        if bar_height == 0:
            continue
        x0 = margin_left + i * bar_width
        y0 = margin_top + chart_height - bar_height
        x1 = x0 + bar_width
        y1 = margin_top + chart_height
        draw.rectangle([x0, y0, x1, y1], fill='#1f77b4', outline='#1f77b4')

    # Заголовок
    try:
        # Попробуем шрифт побольше (на Windows)
        title_font = ImageFont.truetype("arial.ttf", 24)
    except:
        title_font = ImageFont.load_default()
    draw.text((width//2, 15), title, fill='black', font=title_font, anchor='mt')

    # Подписи осей
    try:
        axis_font = ImageFont.truetype("arial.ttf", 18)
    except:
        axis_font = ImageFont.load_default()
    draw.text((width//2, height - 20), "Номер корзины (0..4095)", fill='black', font=axis_font, anchor='ms')
    draw.text((15, height//2), "Количество слов", fill='black', font=axis_font, anchor='mm', angle=90)

    # Сохраняем
    img.save(out_path)
    print(f"Сохранён: {out_path}")

def main():
    out_dir = "histograms_pillow"
    os.makedirs(out_dir, exist_ok=True)

    txt_files = [f for f in os.listdir('.') if f.endswith('.txt')]
    if not txt_files:
        print("Нет .txt файлов с распределением.")
        return

    for fname in txt_files:
        try:
            counts = parse_distribution(fname)
            if not counts or max(counts) == 0:
                print(f"Пропуск {fname}: нет данных")
                continue
            # Имя для заголовка и файла
            name = os.path.splitext(fname)[0]
            out_path = os.path.join(out_dir, f"{name}.png")
            draw_histogram_full(counts, name, out_path)
        except Exception as e:
            print(f"Ошибка с {fname}: {e}")

if __name__ == "__main__":
    main()