#include "myimage.h"

MyImage::MyImage(GrabResultPtr_t pointer) {
    width = pointer->GetWidth();
    height = pointer->GetHeight();
    data = new uint8_t[3*width*height];
    rawData = (uint16_t *)pointer->GetBuffer();
    demosaic();
}

QImage MyImage::toQImage() {
    QImage image(data, width, height, QImage::Format_RGB888);
    return image;
}

MyImage::~MyImage() {
    delete data;
}

QPixmap MyImage::toQPixmap() {
    QPixmap pixmap = QPixmap::fromImage(QImage::QImage(data, width, height, QImage::Format_RGB888));
    return pixmap;
}

// demosaic and convert 16-bit to 8-bit
// NOTE: Only works with BayerBG12 pixel format
void MyImage::demosaic() {
    // non-interpolated values
    for (int i=0; i<height/2; ++i) { // for every other row
        for (int j=0; j<width/2; ++j) { // for every other pixel
            data[6*i*width + 3*width + 6*j + 3] = rawData[2*i*width + width + 2*j + 1]/16; // for each red pixel in pixelData
            data[6*i*width + 6*j + 4] = rawData[2*i*width + 2*j + 1]/16; // for each green pixel in each blue row in pixelData
            data[6*i*width + 3*width + 6*j + 1] = rawData[2*i*width + width + 2*j]/16; // for each green pixel in each red row in pixelData
            data[6*i*width + 6*j + 2] = rawData[2*i*width + 2*j]/16; // for each blue pixel in pixelData
        }
    }
    // interpolate internal values
    for (int i=0; i<(height/2 - 1); ++i) { // for every other row
        for (int j=0; j<(width/2 - 1); ++j) { // for every other pixel
            // Red
            data[6*i*width + 6*j + 3*width + 6] = (rawData[2*i*width + 2*j + width + 1] + rawData[2*i*width + 2*j + width + 3])/32; // red values in green pixels in red rows
            data[6*i*width + 6*j + 6*width + 3] = (rawData[2*i*width + 2*j + width + 1] + rawData[2*i*width + 2*j + 3*width + 1])/32; // red values in green pixels in blue rows
            data[6*i*width + 6*j + 6*width + 6] = (rawData[2*i*width + 2*j + width + 1] + rawData[2*i*width + 2*j + width + 3] + rawData[2*i*width + 2*j + 3*width + 1] + rawData[2*i*width + 2*j + 3*width + 3])/64; // red values in blue pixels
            // Green
            data[6*i*width + 6*j + 3*width + 4] = (rawData[2*i*width + 2*j + 1] + rawData[2*i*width + 2*j + width] + rawData[2*i*width + 2*j + width + 2] + rawData[2*i*width + 2*j + 2*width + 1])/64; // green values in red pixels
            data[6*i*width + 6*j + 6*width + 7] = (rawData[2*i*width + 2*j + width + 2] + rawData[2*i*width + 2*j + 2*width + 1] + rawData[2*i*width + 2*j + 2*width + 3] + rawData[2*i*width + 2*j + 3*width + 2])/64; // green values in blue pixels
            // Blue
            data[6*i*width + 6*j + 3*width + 8] = (rawData[2*i*width + 2*j + 2] + rawData[2*i*width + 2*j + 2*width + 2])/32; // blue values in green pixels in red rows
            data[6*i*width + 6*j + 6*width + 5] = (rawData[2*i*width + 2*j + 2*width] + rawData[2*i*width + 2*j + 2*width + 2])/32; // blue values in green pixels in blue rows
            data[6*i*width + 6*j + 3*width + 5] = (rawData[2*i*width + 2*j] + rawData[2*i*width + 2*j + 2] + rawData[2*i*width + 2*j + 2*width] + rawData[2*i*width + 2*j + 2*width + 2])/64; // blue values in red pixels
        }
    }
    // interpolate top & bottom edges
    for (int j=0; j<(width/2 - 1); ++j) {
        // Red
        data[6*j + 3] = rawData[width + 2*j + 1]/16; // red values in green pixels on top row
        data[3*width*(height-1) + 6*j + 6] = (rawData[width*(height-1) + 2*j + 1] + rawData[width*(height-1) + 2*j + 3])/32; // red values in green pixels on bottom row
        data[6*j + 6] = (rawData[width + 2*j + 1] + rawData[width + 2*j + 3])/32; // red values in blue pixels on top row
        // Green
        data[6*j + 7] = (rawData[2*j + 1] + rawData[2*j + 3] + rawData[width + 2*j + 2])/48; // green values in blue pixels on top row
        data[3*width*(height-1) + 6*j + 4] = (rawData[width*(height-2) + 2*j + 1] + rawData[width*(height-1) + 2*j] + rawData[width*(height-1) + 2*j + 2])/48; // green values in red pixels on bottom row
        // Blue
        data[6*j + 5] = (rawData[2*j] + rawData[2*j + 2])/32; // blue values in green pixels on top row
        data[3*width*(height-1) + 6*j + 8] = rawData[width*(height-2) + 2*j + 2]/16; // blue values in green pixels on bottom row
        data[3*width*(height-1) + 6*j + 5] = (rawData[width*(height-2) + 2*j] + rawData[width*(height-2) + 2*j + 2])/32; // blue values in red pixels on bottom row
    }
    // interpolate left and right edges
    for (int i=0; i<(height/2 - 1); ++i) {
        // Red
        data[6*i*width + 3*width] = rawData[2*i*width + width + 1]/16; // red values in green pixels on left side
        data[6*i*width + 6*width] = (rawData[2*i*width + width + 1] + rawData[2*i*width + 3*width + 1])/32; // red values in blue pixels on left side
        data[6*i*width + 9*width - 3] = (rawData[2*i*width + 2*width - 1] + rawData[2*i*width + 4*width - 1])/32; // red values in green pixels on right side
        // Green
        data[6*i*width + 6*width + 1] = (rawData[2*i*width + width] + rawData[2*i*width + 2*width + 1] + rawData[2*i*width + 3*width])/48; // green values in blue pixels on left side
        data[6*i*width + 6*width - 2] = (rawData[2*i*width + width - 1] + rawData[2*i*width + 2*width - 2] + rawData[2*i*width + 3*width - 1])/48; // green values in red pixels on right side
        // Blue
        data[6*i*width + 3*width + 2] = (rawData[2*i*width] + rawData[2*i*width + 2*width])/32; // blue values in green pixels on left side
        data[6*i*width + 9*width - 1] = rawData[2*i*width + 3*width - 2]/16; // blue values in green pixels on right side
        data[6*i*width + 6*width - 1] = (rawData[2*i*width + width - 2] + rawData[2*i*width + 3*width - 2])/32; // blue values in red pixels on right side
    }
    // interpolate corners
    // upper left
    data[0] = rawData[width+1]/16; // R
    data[1] = (rawData[width] + rawData[2])/32; // G
    // upper right
    data[3*width-3] = rawData[2*width-1]/16; // R
    data[3*width-1] = rawData[width-2]/16; // B
    // lower left
    data[3*width*(height-1)] = rawData[width*(height-1)+1]/16; // R
    data[3*width*(height-1)+2] = rawData[width*(height-2)]/16; // B
    // lower right
    data[3*width*height-2] = (rawData[width*height-2] + rawData[width*(height-1)-1])/32; // G
    data[3*width*height-1] = rawData[width*(height-1)-2]/16; // B
}
