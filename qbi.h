#include <stdint.h>
#ifndef _QBI_H_
#define _QBI_H_
enum  QBI_TYPE {
	QBI_2BIT,
	QBI_3BIT,
	QBI_8BIT,
	QBI_24BIT
};
typedef struct __attribute__((packed)) qbi_header {
	struct {
		uint8_t version :5;
		enum QBI_TYPE type :2;
		uint8_t compression :1;
	} format;

	uint16_t width;
	uint16_t height;
} qbi_header;

void qbi_write_header(int fd, qbi_header* q) {
	uint64_t h = (0x0000000000000000 | ((uint64_t) q->width  << 48) | ((uint64_t) q->height << 32)| q->format.type);
	write(fd, q, sizeof(qbi_header));
}

void qbi_read_header(int fd, qbi_header* q) {
	read(fd, q, sizeof(qbi_header));
}

#endif
