#include <jfc/travis_ci_canary/icons.h>

#include <mutex>

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 32 * 4 + 1];
} travis_ci_32_no_connection = {
  32, 32, 4,
  "\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\324\340Y\001\324\341"
  "W\005\324\341W\007\324\341W\007\324\341W\005\323\337X\001\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\324\340W\024\324\341Wf\324\341W\260\324\341W\335\324\341W\362\324\341"
  "W\362\324\341W\334\324\341W\257\324\341Wf\324\340W\024\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\325"
  "\342X\010\324\341Wu\324\341W\352\324\341W\377\324\341W\377\324\341W\377\324"
  "\341W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\351"
  "\324\341Ww\325\342V\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\000\000\000\000\000\000\000\000\324\340W\030\324\341W\275\324\341W\376\324\341W\377\324"
  "\341W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377"
  "\324\341W\377\324\341W\377\324\341W\377\324\341W\376\324\341W\276\325\342"
  "W\031\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256\256\256\377\256"
  "\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\325\341W\025"
  "\324\341W\315\324\341W\377\324\341W\377\324\341W\377\324\341W\377\313\260"
  "B\377\306\217\064\377\306\217\064\377\306\217\064\377\306\217\064\377\313\260"
  "B\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\314\324"
  "\342X\024\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\324\341"
  "W\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377\306\217\064\377"
  "\301`\037\377\301`\037\377\301`\037\377\301`\037\377\306\217\064\377\324\341W"
  "\377\324\341W\377\324\341W\377\324\341W\377\324\341W\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\324\341W\377\324\341W\377\324\341W\377\320\312M\377\320\312M\377\306\217"
  "\064\377\306\217\064\377\320\312M\377\320\312M\377\324\341W\377\324\341W\377"
  "\324\341W\377\316\332Q\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\324\341W\377\324"
  "\341W\377\324\341W\377\324\341W\377\306\217\064\377\306\217\064\377\324\341"
  "W\377\324\341W\377\324\341W\377\324\341W\377\323\340V\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\272\301\070>\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\324\341W\377\324\341W\377\324\340W\377"
  "\306\216\063\377\306\216\063\377\324\340W\377\324\341W\377\324\341W\377\324"
  "\341W\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\272\301\071=\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\323\341W\006\324\341WX\322\337U\313\323\340V\377\324\341"
  "W\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256"
  "\256\256\377\324\341W\377\316\277H\377\277;\016\377\277;\016\377\316\277H\377"
  "\324\341W\377\324\341W\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\323\340V\377\322\337U\315\324\341WX\323"
  "\340U\006\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\324\341W\220\324"
  "\341W\375\324\341W\377\324\341W\377\324\341W\377\311\323L\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\273"
  "\271\070\377\265\231$\377\265\231$\377\273\271\070\377\277\310@\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\324\341W\377\324\341W\377\324\341W\377\324\341W\375\324\341W\220\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\324\341W\242\324\341W\377\324\341"
  "W\377\324\341W\377\324\341W\377\311\323L\377\257\265,\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\274\304"
  "=\377\274\304=\377\273\303<\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\311\323L\377\324\341W\377\324\341W\377"
  "\324\341W\377\324\341W\377\324\341W\241\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\324\341W\220\324\341W\377\324\341W\377\324\341W\377\324\341"
  "W\377\321\335T\377\315\331P\377\322\337U\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\323\336V\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\315\331P\377\321\335T\377\324\341W\377\324\341W\377\324\341W\377\324\341"
  "W\377\324\341W\217\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\324\341"
  "Wq\324\341W\377\324\341W\377\324\340W\377\322\330U\377\313\300K\377\306\242"
  "B\377\277\220:\377\262u>\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256"
  "\256\256\377\256\256\256\377\304\217\066\377\302\244F\377\300\312a\377\321"
  "\335[\377\324\341W\377\324\341W\377\324\341W\377\324\341Wo\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\324\341WJ\324\340V\376\313\275J\377\303"
  "\203\064\377\276E\027\377\270>!\377\232[W\377\222[W\377\272L\064\377\271th\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\277F*\377\251h_\377pms\377Xpz\377"
  "f{y\377\223\242r\377\276\313a\377\323\340W\376\324\341WG\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\322\336T\024\303\232\064\344\277\071\016\377"
  "\276;\031\377\271sg\377\257\232\225\377\271\214\201\377\274\213\200\377\245"
  "\221\214\377\266\245\237\377\274\252\244\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\275\215\203\377\254"
  "\226\220\377\256\236\231\377\270\250\242\377\250\235\233\377\216\215\217"
  "\377fw\200\377dww\377\276\304E\323\322\337T\024\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\277\066\014\037\277\067\014\355\261E\061\377\227}{\377\274"
  "\252\244\377\274\250\241\377\274\250\241\377\244\226\222\377\215\220\222"
  "\377\211\177}\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377}}\177\377\177\177"
  "\200\377\272\251\243\377\274\252\244\377\274\252\244\377\204\207\213\377"
  "\230\216|\377\375\313\177\264\376\314\200\003\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\277\066\015\002\277\067\015\242\330\177I\376\267\232w\377hy\201\377"
  "\273\251\243\377\274\252\244\377\274\252\244\377\216\203\200\377\060:?\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\064<@\377\260\240\233\377\274\252\244\377\273\251\243\377iy\201\377\274\243"
  "}\377\377\314\200\363\377\314\200\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\277\065\014\064\302B\032\217\376\312~\340\334\266~\377Yp{\377\262\244\240"
  "\377\274\252\244\377\274\252\244\377\241\224\217\377\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\376\337\262"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256"
  "\256\256\377\257\237\232\377\274\252\244\377\262\244\240\377Yp{\377\334\266"
  "~\377\377\314\200\335\377\314\200\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\277\066"
  "\014\001\277\066\014O\312e\067\005\377\314\200\250\363\305\177\377_s{\377\205\210"
  "\213\377\266\246\241\377\271\250\243\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\377\340\262\377\377\340"
  "\262\377\350\317\251\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\266\246\241\377\206\210\214\377_sz\377\363"
  "\305\177\377\377\314\200\247\377\314\201\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\275\070\012\001\275\070\012\006\000\000\000\000\377\314\200\067\376\314\200\356\321\267"
  "\221\376m|\200\377ew\200\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\376\336\260\377\377\340\262\377\377"
  "\340\262\377\376\337\261\377\333\305\243\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\320"
  "\267\220\376\376\314\200\356\377\314\200\067\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\345\236a,\376\331\245\216\360"
  "\325\254\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\325\202^\377\345\250\201\377\376\337\261\377\376\337"
  "\261\377\344\250\201\377\325\204`\377\366\317\242\377\256\256\256\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\345\237a,\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\277\066\014\060\306Z;\310\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\303F#\377\277\066\014"
  "\377\300;\024\377\320xV\377\320wU\377\300:\024\377\277\066\014\377\303G$\377"
  "\341\242{\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\301\066\014\001\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\277\066\015"
  "\377\277\066\014\377\277\066\014\377\277\066\014\377\277\066\014\377\277\066\014\377"
  "\277\066\014\377\277\066\014\377\277\066\014\377\277\066\015\377\301A\037\377\301"
  "C\"\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256"
  "\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\256\256\256\377\277\066\014\377\277\066\014\377\277\066\014\377\277\066\014"
  "\377\277\066\014\377\300\070\015\377\300\070\015\377\277\066\014\377\277\066\014\377"
  "\277\066\014\377\277\066\014\377\277\066\014\377\277\066\014\377\277\066\014\377\256"
  "\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256"
  "\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\300"
  "\070\020\373\277\066\014\377\277\066\014\377\277\066\014\377\277\067\015\377\315\\"
  "\040\377\356\233?\377\356\233@\377\315^!\377\277\067\016\377\277\066\014\377\277"
  "\066\014\377\277\066\014\377\300\070\017\374\277\066\014\277\277\066\014\070\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\302A\035\002\340\233sQ\336"
  "\231s\355\310[\071\377\311_=\377\350\257\206\377\375\305|\377\377\273X\377"
  "\377\273X\377\376\305|\377\351\261\207\377\311_=\377\310[\071\377\335\227"
  "q\357\337\226oR\302@\031\003\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\000\000\000\000\000\000\000\000\000\000\000\000\377\337\262-\376\335\260\300\376\336\260"
  "\375\377\340\262\377\377\340\262\377\377\337\260\377\377\337\260\377\377"
  "\340\262\377\377\340\262\377\376\336\260\375\376\335\257\300\377\340\262"
  "-\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000\256\256\256\377"
  "\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\336\260\004\377\340\262G\377\340\262\237"
  "\377\340\262\326\377\340\262\361\377\340\262\361\377\340\262\326\377\340"
  "\262\237\377\340\262F\377\336\257\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\256\256\256\377"
  "\256\256\256\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\377\340\262\004\377\340\262\006\377\340\262\006\377\340\262\004"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\256\256\256\377\256\256\256\377\256\256\256\377\256\256"
  "\256\377\256\256\256\377\256\256\256\377\256\256\256\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

GdkPixbuf *jfc::get_disconnected_icon_image()
{
    static GdkPixbuf *pixbuf;
    static std::once_flag flag;

    std::call_once(flag, []()
    {
        pixbuf = gdk_pixbuf_new_from_data(
           travis_ci_32_no_connection.pixel_data, //ptr to beginning of data
           GDK_COLORSPACE_RGB, //data has rgb channels (redundant, since enum only has 1 value)
           TRUE, //data has an alpha channel
           8, //each channel has 8 bits (hence rgba32)
           travis_ci_32_no_connection.width, //width in pixels
           travis_ci_32_no_connection.height, //height in pixels
           4 * travis_ci_32_no_connection.width, //stride (row length in bytes: 1 byte per channel * 4 channels * width in pixels)
           nullptr, //destroy functor, use to add user defined clean up
           nullptr //user data for destroy functor
        );
    });

    return pixbuf; 
}
