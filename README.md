# QBI
The "Qonnell Bitmap Image" file format. A simplified Bitmap

## Structure:
40 bit header, followed by data

| Bits 0-4 | Bits 5-6 | Bit 7 | Bits 8-24 | Bits 25-39 |
| -------- | -------- | ------| --------- | ---------- |
| Reserved  | type (Bits) |  Compression (true/false) | Width(px) | Height (px) |

```c
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

## Usage:
#### Creating:
```
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <qbi.h>

int main() {
	int fd = creat("test.qbi", 0);
	//uint64_t head =  0x0320032000000060;
	//write(fd, &head, sizeof(qbi_header));
	qbi_header q = { {0, QBI_24BIT, 0}, 0x320, 0x320};
	qbi_write_header(fd, &q);
	for(int i = 0; i < 0x320; i++) {
		for(int j = 0; j < 0x320; j++) {
			uint32_t rgb = j << 16 | i << 8 | i^j;//rand() % 256;
			//rgb = ~rgb;
			//read(randfd, &rgb, 4);
			write(fd, (uint32_t*)&rgb, 3);
		}
	}
	close(fd);
}
```

#### Displaying (SDL2):
```c
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <qbi.h>

struct __attribute__((packed)) pixel {
	uint8_t r, g, b;
};

int main(int argc, char** argv) {
	qbi_header head;
	int fd = open((argc > 1)? argv[1] : "test.qbi", 0);
	qbi_read_header(fd, &head);
	printf("type: %d, w: %x, h: %x\n", head.format.type, head.width, head.height);
	struct pixel image[head.height][head.width];
	read(fd, image, head.height*head.width*3);
	close(fd);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_CreateWindowAndRenderer(head.width, head.height, 0, &win, &ren);

	uint32_t rmask, gmask, bmask, amask;
/* Roughly taken from STBI source code <https://github.com/nothings/stb> */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000 >> 8;
	gmask = 0x00ff0000 >> 8;
	bmask = 0x0000ff00 >> 8;
	amask = 0x000000ff >> 8;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0;
#endif
	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)image, head.width, head.height, 24, 3*head.width, rmask, gmask, bmask, amask);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_FreeSurface(surf);

	SDL_RenderPresent(ren);
	SDL_Delay(2000);
	SDL_Quit();
	return 0;
}
```
