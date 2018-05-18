# Simple Stenography Example

This app is hiding text into image.

The way it does it is writing UTF-8 int values of char's to RGB of image pixel, exactly replacing the Blue value.

To select pixels are used pseudo-random numbers with Secret Number seed.
To decode text file you need to provide the same Secret Number, which was used to encode.
