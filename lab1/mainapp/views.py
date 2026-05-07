from rest_framework import viewsets
from .models import RaceResult
from .serializers import RaceResultSerializer

class RaceResultViewSet(viewsets.ModelViewSet):
    queryset = RaceResult.objects.all()
    serializer_class = RaceResultSerializer