#ifndef ICON_H
#define ICON_H

class Icon {
public:
	bool loadIcon(const unsigned char *buf);
	void create_texture();
	void drawIcon(float x, float y);
	void getIcon(unsigned char *buf);

	unsigned short pal_to_4444(unsigned int pal);
private:
	unsigned int palette[16];
	unsigned char data[32*16];
	void *icon_texture;
};

#endif
