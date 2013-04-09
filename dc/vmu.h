
#ifdef __cplusplus
extern "C" {
#endif

int vm_SearchFile(const char *fname, int *vm);
void drawlcdstring(int vm, int x, int y, const char *str);
bool vmu_select(int *vm, const char *desc);
bool save_to_vmu(int unit, const char *filename, const char *desc_long, const char *desc_short, void *buf, int buf_len, Icon *icon);
bool load_from_vmu(int unit, const char *filename, void *buf, int *buf_len);

#ifdef __cplusplus
}
#endif
