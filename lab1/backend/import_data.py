import csv
import os
import django
from datetime import datetime

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'lab1.settings')
django.setup()

from mainapp.models import RaceResult

def run_import():
    file_path = 'data/f1_pitstops_2018_2024.csv'
    
    if not os.path.exists(file_path):
        print(f"Ошибка: Файл не найден по пути {file_path}")
        return

    with open(file_path, encoding='utf-8') as f:
        reader = csv.DictReader(f)
        count = 0
        
        for row in reader:
            def to_int(val):
                try: return int(float(val))
                except: return 0

            def to_float(val):
                try: return float(val)
                except: return 0.0

            try:
                date_obj = datetime.strptime(row['Date'], '%Y-%m-%d').date()
            except:
                date_obj = datetime.now().date()

            RaceResult.objects.create(
                season=to_int(row['Season']),
                race_name=row['Race Name'],
                date=date_obj,
                driver=row['Driver'],
                constructor=row['Constructor'],
                laps=to_int(row['Laps']),
                position=to_int(row['Position']),
                avg_pit_time=to_float(row['AvgPitStopTime']),
                track_temp=to_float(row['Track_Temp_C'])
            )
            count += 1
            if count % 100 == 0:
                print(f"Загружено {count} строк...")

    print(f"Готово! Всего импортировано записей: {count}")

if __name__ == '__main__':
    run_import()