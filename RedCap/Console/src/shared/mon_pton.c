00019 
00020 #include <net-snmp/net-snmp-config.h>
00021 
00022 #ifndef HAVE_INET_PTON
00023 
00024 #if HAVE_ARPA_NAMESER_H
00025 #include <arpa/nameser.h>
00026 #endif
00027 
00028   /*
00029    * Net-SNMP Win32 additions
00030    */
00031 #if HAVE_WINSOCK_H
00032 #include <winsock2.h>
00033 #include <errno.h>
00034 #include <stdio.h>
00035 #endif
00036 
00037 #ifndef EAFNOSUPPORT
00038 #define EAFNOSUPPORT            WSAEAFNOSUPPORT
00039 #endif
00040 #ifndef INET6
00041 #define INET6
00042 #endif
00043 
00044 #ifndef IN6ADDRSZ
00045 #define IN6ADDRSZ       16
00046 #endif
00047 
00048 #ifndef INT16SZ
00049 #define INT16SZ         2
00050 #endif
00051   /*
00052    * End of Net-SNMP Win32 additions
00053    */
00054 
00055 #ifndef INADDRSZ
00056 #define INADDRSZ        4
00057 #endif
00058 
00059 /*
00060  * WARNING: Don't even consider trying to compile this on a system where
00061  * sizeof(int) < 4.  sizeof(int) > 4 is fine; all the world's not a VAX.
00062  */
00063 
00064 static int      inet_pton4(const char *src, u_char *dst, int pton);
00065 #ifdef INET6
00066 static int      inet_pton6(const char *src, u_char *dst);
00067 #endif
00068 
00069 /* int
00070  * inet_pton(af, src, dst)
00071  *      convert from presentation format (which usually means ASCII printable)
00072  *      to network format (which is usually some kind of binary format).
00073  * return:
00074  *      1 if the address was valid for the specified address family
00075  *      0 if the address wasn't valid (`dst' is untouched in this case)
00076  *      -1 if some other error occurred (`dst' is untouched in this case, too)
00077  * author:
00078  *      Paul Vixie, 1996.
00079  */
00080 int
00081 inet_pton(af, src, dst)
     00082         int af;
     00083         const char *src;
     00084         void *dst;
     00085 {
00086 
  00087         switch (af) {
    00088         case AF_INET:
		    00089                 return (inet_pton4(src, dst, 1));
00090 #ifdef INET6
  00091         case AF_INET6:
  00092                 return (inet_pton6(src, dst));
00093 #endif
  00094         default:
  00095                 errno = EAFNOSUPPORT;
 00096                 return (-1);
 00097 }
 00098         /* NOTREACHED */
   00099 }
00100 
00101 /* int
00102  * inet_pton4(src, dst, pton)
00103  *      when last arg is 0: inet_aton(). with hexadecimal, octal and shorthand.
00104  *      when last arg is 1: inet_pton(). decimal dotted-quad only.
00105  * return:
00106  *      1 if `src' is a valid input, else 0.
00107  * notice:
00108  *      does not touch `dst' unless it's returning 1.
00109  * author:
00110  *      Paul Vixie, 1996.
00111  */
00112 static int
00113 inet_pton4(src, dst, pton)
     00114         const char *src;
     00115         u_char *dst;
     00116         int pton;
     00117 {
       00118         u_int val;
       00119         u_int digit;
       00120         int base, n;
       00121         unsigned char c;
       00122         u_int parts[4];
       00123         register u_int *pp = parts;
00124 
  00125         c = *src;
 00126         for (;;) {
00127                 /*
00128                  * Collect number up to ``.''.
00129                  * Values are specified as for C:
00130                  * 0x=hex, 0=octal, isdigit=decimal.
00131                  */
  00132                 if (!isdigit(c))
    00133                         return (0);
 00134                 val = 0; base = 10;
 00135                 if (c == '0') {
   00136                         c = *++src;
   00137                         if (c == 'x' || c == 'X')
     00138                                 base = 16, c = *++src;
   00139                         else if (isdigit(c) && c != '9')
     00140                                 base = 8;
   00141 }
 00142                 /* inet_pton() takes decimal only */
   00143                 if (pton && base != 10)
     00144                         return (0);
 00145                 for (;;) {
   00146                         if (isdigit(c)) {
     00147                                 digit = c - '0';
     00148                                 if (digit >= base)
       00149                                         break;
     00150                                 val = (val * base) + digit;
     00151                                 c = *++src;
     00152 } else if (base == 16 && isxdigit(c)) {
       00153                                 digit = c + 10 - (islower(c) ? 'a' : 'A');
       00154                                 if (digit >= 16)
	 00155                                         break;
       00156                                 val = (val << 4) | digit;
       00157                                 c = *++src;
       00158 } else
	 00159                                 break;
   00160 }
 00161                 if (c == '.') {
00162                         /*
00163                          * Internet format:
00164                          *      a.b.c.d
00165                          *      a.b.c   (with c treated as 16 bits)
00166                          *      a.b     (with b treated as 24 bits)
00167                          *      a       (with a treated as 32 bits)
00168                          */
  00169                         if (pp >= parts + 3)
    00170                                 return (0);
 00171                         *pp++ = val;
 00172                         c = *++src;
 00173 } else
   00174                         break;
 00175 }
00176         /*
00177          * Check for trailing characters.
00178          */
  00179         if (c != '\0' && !isspace(c))
    00180                 return (0);
00181         /*
00182          * Concoct the address according to
00183          * the number of parts specified.
00184          */
  00185         n = pp - parts + 1;
 00186         /* inet_pton() takes dotted-quad only.  it does not take shorthand. */
   00187         if (pton && n != 4)
     00188                 return (0);
 00189         switch (n) {
00190 
  00191         case 0:
  00192                 return (0);             /* initial nondigit */
00193 
  00194         case 1:                         /* a -- 32 bits */
  00195                 break;
00196 
  00197         case 2:                         /* a.b -- 8.24 bits */
  00198                 if (parts[0] > 0xff || val > 0xffffff)
    00199                         return (0);
 00200                 val |= parts[0] << 24;
 00201                 break;
00202 
  00203         case 3:                         /* a.b.c -- 8.8.16 bits */
  00204                 if ((parts[0] | parts[1]) > 0xff || val > 0xffff)
    00205                         return (0);
 00206                 val |= (parts[0] << 24) | (parts[1] << 16);
 00207                 break;
00208 
  00209         case 4:                         /* a.b.c.d -- 8.8.8.8 bits */
  00210                 if ((parts[0] | parts[1] | parts[2] | val) > 0xff)
    00211                         return (0);
 00212                 val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
 00213                 break;
 00214 }
 00215         if (dst) {
   00216                 val = htonl(val);
   00217                 memcpy(dst, &val, INADDRSZ);
   00218 }
 00219         return (1);
 00220 }
00221 
00222 #ifdef INET6
00223 /* int
00224  * inet_pton6(src, dst)
00225  *      convert presentation level address to network order binary form.
00226  * return:
00227  *      1 if `src' is a valid [RFC1884 2.2] address, else 0.
00228  * notice:
00229  *      (1) does not touch `dst' unless it's returning 1.
00230  *      (2) :: in a full address is silently ignored.
00231  * credit:
00232  *      inspired by Mark Andrews.
00233  * author:
00234  *      Paul Vixie, 1996.
00235  */
00236 static int
00237 inet_pton6(src, dst)
     00238         const char *src;
     00239         u_char *dst;
     00240 {
       00241         static const char xdigits_l[] = "0123456789abcdef",
	 00242                           xdigits_u[] = "0123456789ABCDEF";
       00243         u_char tmp[IN6ADDRSZ], *tp, *endp, *colonp;
       00244         const char *xdigits, *curtok;
       00245         int ch, saw_xdigit;
       00246         u_int val;
00247 
  00248         memset((tp = tmp), '\0', IN6ADDRSZ);
 00249         endp = tp + IN6ADDRSZ;
 00250         colonp = NULL;
 00251         /* Leading :: requires some special handling. */
   00252         if (*src == ':')
     00253                 if (*++src != ':')
       00254                         return (0);
 00255         curtok = src;
 00256         saw_xdigit = 0;
 00257         val = 0;
 00258         while ((ch = *src++) != '\0') {
   00259                 const char *pch;
00260 
  00261                 if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
    00262                         pch = strchr((xdigits = xdigits_u), ch);
 00263                 if (pch != NULL) {
   00264                         val <<= 4;
   00265                         val |= (pch - xdigits);
   00266                         if (val > 0xffff)
     00267                                 return (0);
   00268                         saw_xdigit = 1;
   00269                         continue;
   00270 }
 00271                 if (ch == ':') {
   00272                         curtok = src;
   00273                         if (!saw_xdigit) {
     00274                                 if (colonp)
       00275                                         return (0);
     00276                                 colonp = tp;
     00277                                 continue;
     00278 } else if (*src == '\0')
       00279                                 return (0);
   00280                         if (tp + INT16SZ > endp)
     00281                                 return (0);
   00282                         *tp++ = (u_char) (val >> 8) & 0xff;
   00283                         *tp++ = (u_char) val & 0xff;
   00284                         saw_xdigit = 0;
   00285                         val = 0;
   00286                         continue;
   00287 }
 00288                 if (ch == '.' && ((tp + INADDRSZ) <= endp) &&
			   00289                     inet_pton4(curtok, tp, 1) > 0) {
   00290                         tp += INADDRSZ;
   00291                         saw_xdigit = 0;
   00292                         break;  /* '\0' was seen by inet_pton4(). */
   00293 }
 00294                 return (0);
 00295 }
 00296         if (saw_xdigit) {
   00297                 if (tp + INT16SZ > endp)
     00298                         return (0);
   00299                 *tp++ = (u_char) (val >> 8) & 0xff;
   00300                 *tp++ = (u_char) val & 0xff;
   00301 }
 00302         if (colonp != NULL) {
00303                 /*
00304                  * Since some memmove()'s erroneously fail to handle
00305                  * overlapping regions, we'll do the shift by hand.
00306                  */
  00307                 const int n = tp - colonp;
 00308                 int i;
00309 
  00310                 if (tp == endp)
    00311                         return (0);
 00312                 for (i = 1; i <= n; i++) {
   00313                         endp[- i] = colonp[n - i];
   00314                         colonp[n - i] = 0;
   00315 }
 00316                 tp = endp;
 00317 }
 00318         if (tp != endp)
   00319                 return (0);
 00320         memcpy(dst, tmp, IN6ADDRSZ);
 00321         return (1);
 00322 }
00323 #endif
00324 
00325 #endif /* HAVE_INET_PTON */
