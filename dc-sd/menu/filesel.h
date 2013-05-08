
enum {
	FDD_FILE = 0,
	HDD_FILE,
	FONT_FILE,
	BIOS_FILE
};

#ifdef __cplusplus
extern "C" {
#endif

void file_browser(int drv, int type);

#ifdef __cplusplus
}
#endif
