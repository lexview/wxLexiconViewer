
#include "Debug.h"

void wxLogHexDump(const wxMemoryBuffer& buffer)
{
    const unsigned char* data = static_cast<const unsigned char*>(buffer.GetData());
    size_t size = buffer.GetDataLen();

    if (size == 0) {
        wxLogDebug("Buffer is empty");
        return;
    }

    wxLogDebug("Buffer (%zu bytes):", size);

    for (size_t i = 0; i < size; i += 16) {
        wxString hexLine;
        wxString asciiLine;
        
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                unsigned char c = data[i + j];
                hexLine += wxString::Format("%02X ", c);
                asciiLine += (c >= 32 && c < 127) ? wxString::Format("%c", c) : ".";
            } else {
                hexLine += "   ";
                asciiLine += " ";
            }
            
            if (j == 7) hexLine += " "; // Разделитель
        }
        
        wxLogDebug("%08zX  %s %s", i, hexLine, asciiLine);
    }
}
