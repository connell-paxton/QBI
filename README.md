# QBI
The "Qonnell Bitmap Image" file format. A simplified Bitmap


## Structure:
40 bit header, followed by data

| Bits 0-4 | Bits 5-6 | Bit 7 | Bits 8-24 | Bits 25-39 |
| -------- | -------- | ------| --------- | ---------- |
| Reserved  | type (Bits) |  Compression (true/false) | Width(px) | Height (px) |

```
/* As defined in qbi.h: */
typedef struct __attribute__((packed)) qbi_header {
	struct {
		uint8_t version :5;
		enum QBI_TYPE type :2;
		uint8_t compression :1;
	} format;
  uint16_t width;
	uint16_t height;
} qbi_header;
```
