main.o: shogiban.h kyokumencode.h sashite.h kifu.h shogidb.h \
 shogirule.h
shogiban.o: shogiban.h
kyokumencode.o: shogiban.h kyokumencode.h
sashite.o: shogiban.h sashite.h
kifu.o: shogiban.h sashite.h kifu.h
shogidb.o: shogiban.h kyokumencode.h sashite.h kifu.h \
 shogidb.h
shogirule.o: shogiban.h kyokumencode.h sashite.h kifu.h \
 shogidb.h shogirule.h