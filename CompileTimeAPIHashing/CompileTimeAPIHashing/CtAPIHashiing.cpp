//#include "CtAPIHashing.h"
//
//constexpr const int get_seed() {
//	return '0' * -49234 +
//		__TIME__[7] * 1 +
//		__TIME__[6] * 10 +
//		__TIME__[5] * 60 +
//		__TIME__[4] * 3600 +
//		__TIME__[3] * 36000;
//}
//
//constexpr int g_KEY = get_seed() % 0xFF;
//
//// Compile Time JenkinsOneAtATime32Bit Wide Function
//constexpr DWORD HashStringDJB2W(const wchar_t* str) {
//	wchar_t c = 0;
//
//	unsigned long hash = (unsigned long)g_KEY;
//
//	while (c = *str++)
//		hash = ((hash << 5) + hash) + c;
//
//	return hash;
//}
//
//// Compile Time JenkinsOneAtATime32Bit Ascii Function
//constexpr DWORD HashStringDJB2A(const char* str) {
//	char c = 0;
//
//	unsigned long hash = (unsigned long)g_KEY;
//
//	while (c = *str++)
//		hash = ((hash << 5) + hash) + c;
//
//	return hash;
//}
//
////#define HashA( API ) HashStringDJB2A((const char *) API )
////#define HashW( API ) HashStringDJB2W((const wchar_t*) API )
////
////#define ctHashA( API ) constexpr auto API##_CTA = HashStringDJB2A((const char *) #API )
////#define ctHashW( API ) constexpr auto API##_CTW = HashStringDJB2W((const wchar_t*) L#API )