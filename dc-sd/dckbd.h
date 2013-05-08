


#ifdef __cplusplus
extern "C" {
#endif

void dckbd_keydown(UINT8 key);
void dckbd_keyup(UINT8 key);

void dckbd_bindinit(void);
void dckbd_bindcur(UINT type);
void dckbd_bindbtn(UINT type);

#ifdef __cplusplus
}
#endif
