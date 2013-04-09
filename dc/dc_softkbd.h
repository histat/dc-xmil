#ifdef __cplusplus
extern "C" {
#endif

void softkbddc_initialize();
void softkbddc_sync();
void softkbddc_send(int button);
void softkbddc_down();
void softkbddc_up();
void softkbddc_draw();
extern BOOL __skbd_avail;
extern BOOL __use_bg;

#ifdef __cplusplus
}
#endif
