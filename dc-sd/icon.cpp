#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "icon.h"
#include <ronin/ta.h>


static unsigned char get_byte(const unsigned char *in) {return in[0];}
static unsigned short get_short(const unsigned char *in) {return in[0]|in[1]<<8;}
static unsigned int get_long(const unsigned char *in) {return in[0]|in[1]<<8|in[2]<<16|in[3]<<24;}

bool Icon::loadIcon(const unsigned char *buf)
{
	unsigned short count;
	unsigned int offset;
	unsigned short BitCount;
	unsigned char ColorCount;
	unsigned char bitmap[32*16];
  
	count = get_short(buf+4);
	ColorCount = get_byte(buf+8);
	offset = get_long(buf+18);
	BitCount = get_short(buf+offset+14);
  
	if (ColorCount != 16 || BitCount != 4)
		return false;
    
	memset(palette, 0, sizeof(palette));
	memset(bitmap, 0, sizeof(bitmap));

	const unsigned char *p = (const unsigned char*)(buf+offset+40);

	for (int i=0; i<16; ++i) {
		palette[i] = get_long(p)|0xff000000;
    
		p += 4;
	}

	const unsigned char *bit = (const unsigned char*)(buf+offset+40+ColorCount*4);
	const unsigned char *mask = (const unsigned char*)(bit+32*16);

	memcpy(bitmap, bit, sizeof(bitmap));
  
	int unused = -1;

	for (int i=0; i<16; ++i) {
		unsigned int n;
		for (n=0; n<sizeof(bitmap); ++n) {
			int pal = bitmap[n];
			if (i == ((pal>>4)&0x0f) || i == (pal&0x0f))
				break;
		}
		if (n >= sizeof(bitmap)) {
			unused = i;
			break;
		}
	}

	if (unused != -1) {
		palette[unused] = 0;
	}
  
	for (int i=0; i<32; ++i) {
		int mbit = mask[0]<<24|mask[1]<<16|mask[2]<<8|mask[3]<<0;
		unsigned int n = 0x80000000;
		for (int j=0; j<16; ++j) {
			if (mbit & n) {
				bitmap[i*16+j] &= 0x0f;
				bitmap[i*16+j] |= (unused&0x0f)<<4;
			}
			if (mbit & (n>>1)) {
				bitmap[i*16+j] &= 0xf0;
				bitmap[i*16+j] |= (unused&0x0f);
			}
			n >>= 2;
		}
		bit += 4;
		mask += 4;
	}

	unsigned char *d = data;
	for (int l=0; l<32; ++l) {
		memcpy(d, bitmap+(16*(31-l)), 16);
		d += 16;
	}

	return true;
}

void Icon::create_texture()
{
	icon_texture = tx_getwork(32*32*2);
	unsigned short *dst = (unsigned short *)icon_texture;

	unsigned short p4444[16];
	for (int i=0; i<16; ++i) {
		p4444[i] = pal_to_4444(palette[i]);
	}
  
	for (int y=0; y<32; ++y) {
		for (int x=0; x<16; ++x) {
			int pix = data[y*16+x];
			*dst++ = p4444[(pix>>4) & 0x0f];
			*dst++ = p4444[(pix>>0) & 0x0f];
		}
	}
}

void Icon::drawIcon(float x, float y)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_TEXTURED|TA_CMD_POLYGON_PACKED_COLOUR;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 = TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_ENABLE_ALPHA|TA_POLYMODE2_FOG_DISABLED|
		TA_POLYMODE2_U_SIZE_32|TA_POLYMODE2_V_SIZE_32;
	mypoly.texture = TA_TEXTUREMODE_ARGB4444|TA_TEXTUREMODE_NON_TWIDDLED
		|TA_TEXTUREMODE_ADDRESS(icon_texture);
	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	ta_commit_list(&mypoly);
  
	myvertex.colour = 0;
	myvertex.ocolour = 0;
	myvertex.z = 0.5;
	myvertex.x = x;
	myvertex.y = y;
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.u = 0.0;
	myvertex.v = 0.0;
	ta_commit_list(&myvertex);
  
	myvertex.u = 1.0;
	myvertex.x = x+32;
	ta_commit_list(&myvertex);
  
	myvertex.u = 0.0;
	myvertex.v = 1.0;
	myvertex.x = x;
	myvertex.y = y+32;
	ta_commit_list(&myvertex);
  
	myvertex.u = 1.0;
	myvertex.x = x+32;
	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	ta_commit_list(&myvertex);
}

unsigned short Icon::pal_to_4444(unsigned int pal)
{
	unsigned short r;

	r  = (pal>>16)& 0xf000;
	r |= (pal>>12)& 0x0f00;
	r |= (pal>>8) & 0x00f0;
	r |= (pal>>4) & 0x000f;
  
	return r;
}

void Icon::getIcon(unsigned char *buf)
{
	unsigned short *p = (unsigned short *)buf;

	for (int i=0; i<16; ++i) {
		p[i] = pal_to_4444(palette[i]);
	}

	memcpy(buf+32, data, sizeof(data));
}
