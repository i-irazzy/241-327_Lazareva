from rest_framework import serializers
from .models import RaceResult

class RaceResultSerializer(serializers.ModelSerializer):
    class Meta:
        model = RaceResult
        fields = '__all__'