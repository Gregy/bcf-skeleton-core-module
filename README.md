# Dead simple atmo value exporter for BigClown

Requires:

- core module
- co2 module
- humidity tag

Exports values over the usb serial like this:
```
co2:738.00
humidity:48.74
temperature:23.61
```

every 30 seconds

Included script serialToFiles.sh can be used to capture the serial output and store the values to local files for further processing.


