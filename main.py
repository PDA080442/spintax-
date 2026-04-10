import tkinter as tk
from tkinter import filedialog
import re
import random
import os

def load_replacements(filename):
    replacements = {}
    with open(filename, 'r', encoding='utf-8') as file:
        for line in file:
            line = line.strip().split('::')
            phrase_to_replace = line[0].strip()
            replacement_options = line[1].strip().split('..')
            replacements[phrase_to_replace] = replacement_options
    return replacements

def process_spin_text(text, replacements):
    for phrase_to_replace, replacement_options in replacements.items():
        pattern = re.compile(re.escape(phrase_to_replace))
        matches = pattern.finditer(text)
        for match in matches:
            start = match.start()
            end = match.end()
            phrase = text[start:end]
            phrase_to_replace = phrase_to_replace.replace('<', '').replace('>', '')  # убираем символы "<" и ">"
            replacement = random.choice(replacement_options)
            text = text[:start] + replacement + text[end:]
    text = text.replace('<', '').replace('>', '')  # убираем оставшиеся символы "<" и ">"
    return text

def browse_input_file():
    input_file_path = filedialog.askopenfilename(title="Выбрать файл input.txt", filetypes=[("Text files", "*.txt")])
    input_file_entry.delete(0, tk.END)
    input_file_entry.insert(0, input_file_path)

def browse_replacements_file():
    replacements_file_path = filedialog.askopenfilename(title="Выбрать файл replacements.txt", filetypes=[("Text files", "*.txt")])
    replacements_file_entry.delete(0, tk.END)
    replacements_file_entry.insert(0, replacements_file_path)

def process_files():
    input_file = input_file_entry.get()
    replacements_file = replacements_file_entry.get()
    output_file = 'output.txt'  # Файл для сохранения результата

    # Удаление содержимого файла output.txt, если он существует
    if os.path.exists(output_file):
        os.remove(output_file)

    # Загрузка замен из файла
    replacements = load_replacements(replacements_file)

    # Обработка исходного текста и сохранение вариантов текста в файл
    with open(input_file, 'r', encoding='utf-8') as file:
        text = file.read()

    with open(output_file, 'a', encoding='utf-8') as file:
        for _ in range(5):
            processed_text = process_spin_text(text, replacements)
            file.write(processed_text + '\n\n\n\n\n')

    result_label.config(text="Создано 5 вариантов измененного текста в файле output.txt")

# Создание главного окна
root = tk.Tk()
root.title("SpinText Processor")
root.geometry("600x200")  

# Поле для отображения пути к файлу input.txt
input_file_entry = tk.Entry(root)
input_file_entry.grid(row=0, column=0, padx=10, pady=10, sticky='ew')

# Кнопка для выбора файла input.txt
input_file_button = tk.Button(root, text="Добавить входной файл", command=browse_input_file)
input_file_button.grid(row=0, column=1, padx=10, pady=10)

# Поле для отображения пути к файлу replacements.txt
replacements_file_entry = tk.Entry(root)
replacements_file_entry.grid(row=1, column=0, padx=10, pady=10, sticky='ew')

# Кнопка для выбора файла replacements.txt
replacements_file_button = tk.Button(root, text="Добавить файл с заменой", command=browse_replacements_file)
replacements_file_button.grid(row=1, column=1, padx=10, pady=10)

process_button = tk.Button(root, text="Обработать файлы", command=process_files)
process_button.grid(row=2, column=0, columnspan=2, pady=10, sticky='ew')

# Вывод результата
result_label = tk.Label(root, text="", fg="blue")
result_label.grid(row=3, column=0, columnspan=2)

# Можно растянуть диалоговое окно
root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=1)

root.mainloop()
