
WX_CONFIG := wx-config

WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
WX_LIBS := $(shell $(WX_CONFIG) --libs)

BIN2C := bin2c

OBJECTS := Lexicon.o LexiconView.o LexiconFrame.o LexiconDocumentParser.o LexiconDocument.o FontManager.o Debug.o

FONTS := Font0.h Font1.h Font2.h Font3.h Font4.h Font5.h Font6.h Font7.h Font8.h Font9.h

Lexicon: $(OBJECTS) $(FONTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(WX_LIBS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -g -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

$(FONTS): %.h: %.fnt
	$(BIN2C) -H $@ $<

clean:
	@rm -rf *.o
