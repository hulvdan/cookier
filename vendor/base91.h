 // Do this:
 //    #define BASE91_IMPLEMENTATION
 // before you include this file in *one* C or C++ file to create the implementation.

#ifndef BASE91_INCLUDE_H
#define BASE91_INCLUDE_H

#include <stddef.h>

// !banner: INTERFACE
// ██╗███╗   ██╗████████╗███████╗██████╗ ███████╗ █████╗  ██████╗███████╗
// ██║████╗  ██║╚══██╔══╝██╔════╝██╔══██╗██╔════╝██╔══██╗██╔════╝██╔════╝
// ██║██╔██╗ ██║   ██║   █████╗  ██████╔╝█████╗  ███████║██║     █████╗
// ██║██║╚██╗██║   ██║   ██╔══╝  ██╔══██╗██╔══╝  ██╔══██║██║     ██╔══╝
// ██║██║ ╚████║   ██║   ███████╗██║  ██║██║     ██║  ██║╚██████╗███████╗
// ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝

#ifdef __cplusplus
extern "C" {
#endif

struct base91__basE91 {  ///
	unsigned long queue;
	unsigned int nbits;
	int val;
};

void base91__init(struct base91__basE91 *);

size_t base91__encode(struct base91__basE91 *, const void *, size_t, void *);

size_t base91__encode_end(struct base91__basE91 *, void *);

size_t base91__decode(struct base91__basE91 *, const void *, size_t, void *);

size_t base91__decode_end(struct base91__basE91 *, void *);

#ifdef __cplusplus
}
#endif

#endif	// BASE91_INCLUDE_H

// !banner: IMPL
// ██╗███╗   ███╗██████╗ ██╗
// ██║████╗ ████║██╔══██╗██║
// ██║██╔████╔██║██████╔╝██║
// ██║██║╚██╔╝██║██╔═══╝ ██║
// ██║██║ ╚═╝ ██║██║     ███████╗
// ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝

#ifdef BASE91_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

const unsigned char base91__enctab[91] = {  ///
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '#', '$',
	'%', '&', '(', ')', '*', '+', ',', '.', '/', ':', ';', '<', '=',
	'>', '?', '@', '[', ']', '^', '_', '`', '{', '|', '}', '~', '"'
};

const unsigned char base91__dectab[256] = {  ///
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 62, 90, 63, 64, 65, 66, 91, 67, 68, 69, 70, 71, 91, 72, 73,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 74, 75, 76, 77, 78, 79,
	80,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 81, 91, 82, 83, 84,
	85, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 86, 87, 88, 89, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91
};

void base91__init(struct base91__basE91 *b) {  ///
	b->queue = 0;
	b->nbits = 0;
	b->val = -1;
}

size_t base91__encode(struct base91__basE91 *b, const void *i, size_t len, void *o) {  ///
	const unsigned char *ib = (const unsigned char*)i;
	unsigned char *ob = (unsigned char*)o;
	size_t n = 0;

	while (len--) {
		b->queue |= *ib++ << b->nbits;
		b->nbits += 8;
		if (b->nbits > 13) {	/* enough bits in queue */
			unsigned int val = b->queue & 8191;

			if (val > 88) {
				b->queue >>= 13;
				b->nbits -= 13;
			} else {	/* we can take 14 bits */
				val = b->queue & 16383;
				b->queue >>= 14;
				b->nbits -= 14;
			}
			ob[n++] = base91__enctab[val % 91];
			ob[n++] = base91__enctab[val / 91];
		}
	}

	return n;
}

/* process remaining bits from bit queue; write up to 2 bytes */

size_t base91__encode_end(struct base91__basE91 *b, void *o) {  ///
	unsigned char *ob = (unsigned char*)o;
	size_t n = 0;

	if (b->nbits) {
		ob[n++] = base91__enctab[b->queue % 91];
		if (b->nbits > 7 || b->queue > 90)
			ob[n++] = base91__enctab[b->queue / 91];
	}
	b->queue = 0;
	b->nbits = 0;
	b->val = -1;

	return n;
}

size_t base91__decode(struct base91__basE91 *b, const void *i, size_t len, void *o) {  ///
	const unsigned char *ib = (const unsigned char*)i;
	unsigned char *ob = (unsigned char*)o;
	size_t n = 0;
	unsigned int d;

	while (len--) {
		d = base91__dectab[*ib++];
		if (d == 91)
			continue;	/* ignore non-alphabet chars */
		if (b->val == -1)
			b->val = d;	/* start next value */
		else {
			b->val += d * 91;
			b->queue |= b->val << b->nbits;
			b->nbits += (b->val & 8191) > 88 ? 13 : 14;
			do {
				ob[n++] = b->queue;
				b->queue >>= 8;
				b->nbits -= 8;
			} while (b->nbits > 7);
			b->val = -1;	/* mark value complete */
		}
	}

	return n;
}

/* process remaining bits; write at most 1 byte */

size_t base91__decode_end(struct base91__basE91 *b, void *o) {  ///
	unsigned char *ob = (unsigned char*)o;
	size_t n = 0;

	if (b->val != -1)
		ob[n++] = b->queue | b->val << b->nbits;
	b->queue = 0;
	b->nbits = 0;
	b->val = -1;

	return n;
}

#ifdef __cplusplus
}
#endif

#endif // BASE91_IMPLEMENTATION


///
