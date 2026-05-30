from django.db import models

class RaceResult(models.Model):
    season = models.IntegerField(verbose_name="Сезон")
    race_name = models.CharField(max_length=255, verbose_name="Гонка")
    date = models.DateField(verbose_name="Дата")
    driver = models.CharField(max_length=100, verbose_name="Пилот")
    constructor = models.CharField(max_length=100, verbose_name="Команда")
    laps = models.IntegerField(default=0, verbose_name="Круги")
    position = models.IntegerField(null=True, blank=True, verbose_name="Позиция")
    avg_pit_time = models.FloatField(null=True, blank=True, verbose_name="Среднее время пит-стопа")
    track_temp = models.FloatField(null=True, verbose_name="Температура трассы")

    def __str__(self):
        return f"{self.race_name} - {self.driver}"