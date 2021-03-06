/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -  This software is distributed in the hope that it will be
 -  useful, but with NO WARRANTY OF ANY KIND.
 -  No author or distributor accepts responsibility to anyone for the
 -  consequences of using this software, or for whether it serves any
 -  particular purpose or works at all, unless he or she says so in
 -  writing.  Everyone is granted permission to copy, modify and
 -  redistribute this source code, for commercial or non-commercial
 -  purposes, with the following restrictions: (1) the origin of this
 -  source code must not be misrepresented; (2) modified versions must
 -  be plainly marked as such; and (3) this notice may not be removed
 -  or altered from any source or modified source distribution.
 *====================================================================*/


/*
 *  utils.c
 *
 *       error, warning and info procs; all invoked by macros
 *           l_int32    l_errorInt()
 *           l_float32  l_errorFloat()
 *           void      *l_errorPtr()
 *           void       l_errorVoid()
 *           void       l_warning()
 *           void       l_warningInt()
 *           void       l_info()
 *           void       l_infoInt()
 *           void       l_infoInt2()
 *           void       l_infoFloat()
 *           void       l_infoFloat2()
 *
 *       safe string procs
 *           char      *stringNew()
 *           l_int32    stringReplace()
 *           char      *stringJoin()
 *           char      *strtokSafe()
 *
 *       find and replace procs
 *           char      *stringRemoveChars()
 *           char      *stringReplaceSubstr()
 *           char      *stringReplaceEachSubstr()
 *           l_int32    arrayFindSequence()
 *
 *       safe realloc
 *           void      *reallocNew()
 *
 *       read file to memory
 *           l_uint8   *arrayRead()
 *           l_uint8   *arrayReadStream()
 *           l_int32    nbytesInFile()
 *
 *       write memory to file
 *           l_int32    arrayWrite()
 *
 *       byte-swapping data conversion
 *           l_uint16   convertOnBigEnd16()
 *           l_uint32   convertOnBigEnd32()
 *           l_uint16   convertOnLittleEnd16()
 *           l_uint32   convertOnLittleEnd32()
 *
 *       file opening
 *           FILE      *fopenReadStream()
 *
 *       file name operations
 *           l_int32    splitPathAtDirectory()
 *           l_int32    splitPathAtExtension()
 *           char      *genPathname()
 *
 *       timing
 *           void       startTimer()
 *           l_float32  stopTimer()
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "allheaders.h"


/*----------------------------------------------------------------------*
 *                 Error, warning and info message procs                *
 *                                                                      *
 *            ---------------------  N.B. ---------------------         *
 *                                                                      *
 *    (1) These functions all print messages to stderr.                 *
 *                                                                      *
 *    (2) They must be invoked only by macros, which are in             *
 *        environ.h, so that the print output can be disabled           *
 *        at compile time, using -DNO_CONSOLE_IO.                       *
 *                                                                      *
 *----------------------------------------------------------------------*/
/*!
 *  l_errorInt()
 *
 *      Input:  msg (error message)
 *              procname
 *              ival (return val)
 *      Return: ival (typically 1)
 */
l_int32
l_errorInt(const char  *msg, 
           const char  *procname, 
           l_int32      ival)
{
    fprintf(stderr, "Error in %s: %s\n", procname, msg);
    return ival;
}


/*!
 *  l_errorFloat()
 *
 *      Input:  msg (error message)
 *              procname
 *              fval (return val)
 *      Return: fval
 */
l_float32
l_errorFloat(const char  *msg, 
             const char  *procname, 
             l_float32    fval)
{
    fprintf(stderr, "Error in %s: %s\n", procname, msg);
    return fval;
}


/*!
 *  l_errorPtr()
 *
 *      Input:  msg (error message)
 *              procname
 *              pval  (return val)
 *      Return: pval (typically null)
 */
void *
l_errorPtr(const char  *msg,
           const char  *procname, 
           void        *pval)
{
    fprintf(stderr, "Error in %s: %s\n", procname, msg);
    return pval;
}


/*!
 *  l_errorVoid()
 *
 *      Input: msg (error message)
 *             procname
 */
void
l_errorVoid(const char  *msg, 
            const char  *procname)
{
    fprintf(stderr, "Error in %s: %s\n", procname, msg);
    return;
}


/*!
 *  l_warning()
 *
 *      Input: msg (warning message)
 *             procname
 */
void
l_warning(const char  *msg,
          const char  *procname)
{
    fprintf(stderr, "Warning in %s: %s\n", procname, msg);
    return;
}


/*!
 *  l_warningInt()
 *
 *      Input: msg (warning message)
 *             procname
 *             ival (embedded in warning message via %d)
 */
void
l_warningInt(const char  *msg,
             const char  *procname, 
             l_int32      ival)
{
l_int32  bufsize;
char    *charbuf;

    if (!msg || !procname) {
        ERROR_VOID("msg or procname not defined in l_warningInt()", procname);
        return;
    }

    bufsize = strlen(msg) + strlen(procname) + 128;
    if ((charbuf = (char *)CALLOC(bufsize, sizeof(char))) == NULL) {
        ERROR_VOID("charbuf not made in l_warningInt()", procname);
        return;
    }

    sprintf(charbuf, "Warning in %s: %s\n", procname, msg);
    fprintf(stderr, charbuf, ival);

    FREE(charbuf);
    return;
}


/*!
 *  l_info()
 *
 *      Input: msg (info message)
 *             procname
 */
void
l_info(const char  *msg,
       const char  *procname)
{
    fprintf(stderr, "Info in %s: %s\n", procname, msg);
    return;
}


/*!
 *  l_infoInt()
 *
 *      Input: msg (info message)
 *             procname
 *             ival (embedded in info message via %d)
 */
void
l_infoInt(const char  *msg,
          const char  *procname, 
          l_int32      ival)
{
l_int32  bufsize;
char    *charbuf;

    if (!msg || !procname) {
        ERROR_VOID("msg or procname not defined in l_infoInt()", procname);
        return;
    }

    bufsize = strlen(msg) + strlen(procname) + 128;
    if ((charbuf = (char *)CALLOC(bufsize, sizeof(char))) == NULL) {
        ERROR_VOID("charbuf not made in l_infoInt()", procname);
        return;
    }

    sprintf(charbuf, "Info in %s: %s\n", procname, msg);
    fprintf(stderr, charbuf, ival);

    FREE(charbuf);
    return;
}


/*!
 *  l_infoInt2()
 *
 *      Input: msg (info message)
 *             procname
 *             ival1, ival2 (two args, embedded in info message via %d)
 */
void
l_infoInt2(const char  *msg,
           const char  *procname, 
           l_int32      ival1,
           l_int32      ival2)
{
l_int32  bufsize;
char    *charbuf;

    if (!msg || !procname) {
        ERROR_VOID("msg or procname not defined in l_infoInt2()", procname);
        return;
    }

    bufsize = strlen(msg) + strlen(procname) + 128;
    if ((charbuf = (char *)CALLOC(bufsize, sizeof(char))) == NULL) {
        ERROR_VOID("charbuf not made in l_infoInt2()", procname);
        return;
    }

    sprintf(charbuf, "Info in %s: %s\n", procname, msg);
    fprintf(stderr, charbuf, ival1, ival2);

    FREE(charbuf);
    return;
}


/*!
 *  l_infoFloat()
 *
 *      Input: msg (info message)
 *             procname
 *             fval (embedded in info message via %f)
 */
void
l_infoFloat(const char  *msg,
            const char  *procname, 
            l_float32    fval)
{
l_int32  bufsize;
char    *charbuf;

    if (!msg || !procname) {
        ERROR_VOID("msg or procname not defined in l_infoFloat()", procname);
        return;
    }

    bufsize = strlen(msg) + strlen(procname) + 128;
    if ((charbuf = (char *)CALLOC(bufsize, sizeof(char))) == NULL) {
        ERROR_VOID("charbuf not made in l_infoFloat()", procname);
        return;
    }

    sprintf(charbuf, "from %s: %s\n", procname, msg);
    fprintf(stderr, charbuf, fval);

    FREE(charbuf);
    return;
}


/*!
 *  l_infoFloat2()
 *
 *      Input: msg (info message)
 *             procname
 *             fval1, fval2 (two args, embedded in info message via %f)
 */
void
l_infoFloat2(const char  *msg,
             const char  *procname, 
             l_float32    fval1,
             l_float32    fval2)
{
l_int32  bufsize;
char    *charbuf;

    if (!msg || !procname) {
        ERROR_VOID("msg or procname not defined in l_infoFloat2()", procname);
        return;
    }

    bufsize = strlen(msg) + strlen(procname) + 128;
    if ((charbuf = (char *)CALLOC(bufsize, sizeof(char))) == NULL) {
        ERROR_VOID("charbuf not made in l_infoFloat()", procname);
        return;
    }

    sprintf(charbuf, "from %s: %s\n", procname, msg);
    fprintf(stderr, charbuf, fval1, fval2);

    FREE(charbuf);
    return;
}



/*--------------------------------------------------------------------*
 *                       Safe string operations                       *
 *--------------------------------------------------------------------*/
/*!
 *  stringNew()
 *
 *      Input:  src string
 *      Return: dest copy of src string, or null on error
 */
char *
stringNew(const char  *src) 
{
char  *dest;

    PROCNAME("stringNew");

    if (!src)
        return (char *)ERROR_PTR("src not defined", procName, NULL);
    
    if ((dest = (char *)CALLOC(strlen(src) + 2, sizeof(char))) == NULL)
        return (char *)ERROR_PTR("dest not made", procName, NULL);
    strcpy(dest, src);

    return dest;
}
    

/*!
 *  stringReplace()
 *
 *      Input:  &dest string (<return> copy)
 *              src string
 *      Return: 0 if OK; 1 on error
 *
 *  Notes:
 *      (1) Frees any existing dest string
 *      (2) Puts a copy of src string in the dest
 *      (3) If either or both strings are null, does the reasonable thing.
 */
l_int32
stringReplace(char       **pdest,
              const char  *src)
{
char  *scopy;

    PROCNAME("stringReplace");

    if (!pdest)
        return ERROR_INT("pdest not defined", procName, 1);

    if (*pdest)
        FREE(*pdest);
    
    if (src) {
        if ((scopy = (char *)CALLOC(strlen(src) + 2, sizeof(char))) == NULL)
            return ERROR_INT("scopy not made", procName, 1);
        strcpy(scopy, src);
        *pdest = scopy;
    }
    else
        *pdest = NULL;

    return 0;
}
    

/*!
 *  stringJoin()
 *
 *      Input:  src1 string (<optional>)
 *              src2 string (<optional>)
 *      Return: concatenated string, or null on error
 *
 *  Notes:
 *      (1) This is the safe version of strcat; it makes a new string.
 *      (2) It is not an error if either or both of the strings
 *          are empty, or if either or both the pointers are null.
 */
char *
stringJoin(const char  *src1, 
           const char  *src2)
{
char    *dest;
l_int32  srclen1, srclen2, destlen;

    PROCNAME("stringJoin");

    srclen1 = srclen2 = 0;
    if (src1)
        srclen1 = strlen(src1);
    if (src2)
        srclen2 = strlen(src2);
    destlen = srclen1 + srclen2 + 3;

    if ((dest = (char *)CALLOC(destlen, sizeof(char))) == NULL)
        return (char *)ERROR_PTR("calloc fail for dest", procName, NULL);

    if (src1)
        strcpy(dest, src1);
    if (src2)
        strcat(dest, src2);
    return dest;
}


/*!
 *  strtokSafe()
 *
 *      Input:  cstr (input string to be parsed;
 *                    use NULL after the first call)
 *              seps (a string of character separators)
 *              &saveptr (<return> ptr to the next char after
 *                        the last encountered separator)
 *      Return: substr (a new string from the previous saveptr to the first
 *                      separator character), or NULL if end of cstr.
 *
 *  Notes:
 *      (1) This is a thread-safe implementation of strtok.
 *      (2) It has the same interface as strtok_r.
 *      (3) It differs from strtok_r in usage in two respects:
 *          (a) the input string is not altered
 *          (b) each returned substring is newly allocated and must
 *              be freed after use.
 *      (4) Let me repeat that.  This is "safe" because the input
 *          string is not altered and because each returned string
 *          is newly allocated on the heap.
 *      (5) It is here because, surprisingly, some C libraries don't
 *          include strtok_r.
 *      (6) Important usage points:
 *          - Input the string to be parsed on the first invocation.
 *          - Then input NULL after that; the value returned in saveptr
 *            is used in all subsequent calls.
 *      (7) This is only slightly slower than strtok_k.
 */
char *
strtokSafe(char        *cstr,
           const char  *seps,
           char       **psaveptr)
{
char     nextc;
char    *start, *substr;
l_int32  istart, i, j, nchars;

    PROCNAME("strtokSafe");

    if (!seps)
        return (char *)ERROR_PTR("seps not defined", procName, NULL);
    if (!psaveptr)
        return (char *)ERROR_PTR("&saveptr not defined", procName, NULL);

    if (!cstr)
        start = *psaveptr;
    else
        start = cstr;
    if (!start)  /* nothing to do */
        return NULL;

        /* First time, scan for the first non-sep character */
    istart = 0;
    if (cstr) {
        for (istart = 0;; istart++) {
            if ((nextc = start[istart]) == '\0') {
                *psaveptr = NULL;  /* in case caller doesn't check ret value */
                return NULL;
            }
            if (!strchr(seps, nextc))
                break;
        }
    }

        /* Scan through, looking for a sep character; if none is
         * found, 'i' will be at the end of the string. */
    for (i = istart;; i++) {
        if ((nextc = start[i]) == '\0')
            break;
        if (strchr(seps, nextc))
            break;
    }

        /* Save the substring */
    nchars = i - istart;
    substr = (char *)CALLOC(nchars + 1, sizeof(char));
    strncpy(substr, start + istart, nchars);

        /* Look for the next non-sep character.
         * If this is the last substring, return a null saveptr. */
    for (j = i;; j++) {
        if ((nextc = start[j]) == '\0') {
            *psaveptr = NULL;  /* no more non-sep characters */
            break;
        }
        if (!strchr(seps, nextc)) {
            *psaveptr = start + j;  /* start here on next call */
                break;
        }
    }

    return substr;
}


/*--------------------------------------------------------------------*
 *                       Find and replace procs                       *
 *--------------------------------------------------------------------*/
/*!
 *  stringRemoveChars()
 *
 *      Input:  src (input string; can be of zero length)
 *              remchars  (string of chars to be removed from src)
 *      Return: dest (string with specified chars removed), or null on error
 */
char *
stringRemoveChars(const char  *src,
                  const char  *remchars) 
{
char     ch;
char    *dest;
l_int32  nsrc, i, k;

    PROCNAME("stringRemoveChars");

    if (!src)
        return (char *)ERROR_PTR("src not defined", procName, NULL);
    if (!remchars)
        return stringNew(src);
    
    if ((dest = (char *)CALLOC(strlen(src) + 1, sizeof(char))) == NULL)
        return (char *)ERROR_PTR("dest not made", procName, NULL);
    nsrc = strlen(src);
    for (i = 0, k = 0; i < nsrc; i++) {
        ch = src[i];
        if (!strchr(remchars, ch))
            dest[k++] = ch;
    }

    return dest;
}


/*!
 *  stringReplaceSubstr()
 *
 *      Input:  src (input string; can be of zero length)
 *              sub1 (substring to be replaced)
 *              sub2 (substring to put in; can be "")
 *              &found (<return optional> 1 if sub1 is found; 0 otherwise)
 *              &loc (<return optional> location of ptr after replacement)
 *
 *      Return: dest (string with substring replaced), or null if the
 *              substring not found or on error.
 *
 *  Notes:
 *      (1) Replaces the first instance.
 *      (2) To only remove sub1, use "" for sub2
 *      (3) Returns a new string if sub1 and sub2 are the same.
 *      (4) The optional loc is input as the byte offset within the src
 *          from which the search starts, and after the search it is the
 *          char position in the string of the next character after
 *          the substituted string.
 *      (5) N.B. If ploc is not null, loc must always be initialized.
 *          To search the string from the beginning, set loc = 0.
 */
char *
stringReplaceSubstr(const char  *src,
                    const char  *sub1,
                    const char  *sub2,
                    l_int32     *pfound,
                    l_int32     *ploc)
{
char    *ptr, *dest;
l_int32  nsrc, nsub1, nsub2, len, npre, loc;

    PROCNAME("stringReplaceSubstr");

    if (!src)
        return (char *)ERROR_PTR("src not defined", procName, NULL);
    if (!sub1)
        return (char *)ERROR_PTR("sub1 not defined", procName, NULL);
    if (!sub2)
        return (char *)ERROR_PTR("sub2 not defined", procName, NULL);
    
    if (pfound)
        *pfound = 0;
    if (ploc)
        loc = *ploc;
    else
        loc = 0;
    if ((ptr = strstr(src + loc, sub1)) == NULL) {
        return NULL;
    }

    if (pfound)
        *pfound = 1;
    nsrc = strlen(src);
    nsub1 = strlen(sub1);
    nsub2 = strlen(sub2);
    len = nsrc + nsub2 - nsub1;
    if ((dest = (char *)CALLOC(len + 1, sizeof(char))) == NULL)
        return (char *)ERROR_PTR("dest not made", procName, NULL);
    npre = ptr - src;
    memcpy(dest, src, npre);
    strcpy(dest + npre, sub2);
    strcpy(dest + npre + nsub2, ptr + nsub1);
    if (ploc)
        *ploc = npre + nsub2;

    return dest;
}


/*!
 *  stringReplaceEachSubstr()
 *
 *      Input:  src (input string; can be of zero length)
 *              sub1 (substring to be replaced)
 *              sub2 (substring to put in; can be "")
 *              &count (<optional return > the number of times that sub1
 *                      is found in src; 0 if not found)
 *      Return: dest (string with substring replaced), or null if the
 *              substring not found or on error.
 *
 *  Notes:
 *      (1) Replaces every instance.
 *      (2) To only remove each instance of sub1, use "" for sub2
 *      (3) Returns NULL if sub1 and sub2 are the same.
 */
char *
stringReplaceEachSubstr(const char  *src,
                        const char  *sub1,
                        const char  *sub2,
                        l_int32     *pcount)
{
char    *currstr, *newstr;
l_int32  loc;

    PROCNAME("stringReplaceEachSubstr");

    if (!src)
        return (char *)ERROR_PTR("src not defined", procName, NULL);
    if (!sub1)
        return (char *)ERROR_PTR("sub1 not defined", procName, NULL);
    if (!sub2)
        return (char *)ERROR_PTR("sub2 not defined", procName, NULL);

    if (pcount)
        *pcount = 0;
    loc = 0;
    if ((newstr = stringReplaceSubstr(src, sub1, sub2, NULL, &loc)) == NULL)
        return NULL;

    if (pcount)
        (*pcount)++;
    while (1) {
        currstr = newstr;
        newstr = stringReplaceSubstr(currstr, sub1, sub2, NULL, &loc);
        if (!newstr)
            return currstr;
        FREE(currstr);
        if (pcount)
            (*pcount)++;
    }
}
            

/*!
 *  arrayFindSequence()
 *
 *      Input:  data (byte array)
 *              datalen (length of data, in bytes)
 *              sequence (subarray of bytes to find in data)
 *              seqlen (length of sequence, in bytes)
 *              &offset (<return> offset from beginning of data where
 *                       the sequence begins)
 *              &found (<return> 1 if sequence is found; 0 otherwise)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) The byte arrays 'data' and 'sequence' are not C strings,
 *          as they can contain null bytes.  Therefore, for each
 *          we must give the length of the array.
 *      (2) This searches for the first occurrence in 'data' of
 *          the first 'seqlen' bytes of 'sequence'.  The parameter 'seqlen'
 *          must not exceed the actual length of the 'sequence' byte array.
 *      (3) The caller should always check the return value of 'found'.
 *          If not found, the offset will be set to 0.
 */
l_int32
arrayFindSequence(const l_uint8  *data,
                  l_int32         datalen,
                  const l_uint8  *sequence,
                  l_int32         seqlen,
                  l_int32        *poffset,
                  l_int32        *pfound)
{
l_int32  i, j, found, lastpos;

    PROCNAME("arrayFindSequence");

    if (!data || !sequence)
        return ERROR_INT("data & sequence not both defined", procName, 1);
    if (!poffset || !pfound)
        return ERROR_INT("&offset and &found not both defined", procName, 1);

    *poffset = *pfound = 0;
    lastpos = datalen - seqlen + 1;
    found = 0;
    for (i = 0; i < lastpos; i++) {
        for (j = 0; j < seqlen; j++) {
            if (data[i + j] != sequence[j])
                 break;
            if (j == seqlen - 1)
                 found = 1;
        }
        if (found)
            break;
    }

    if (found) {
        *pfound = 1;
        *poffset = i;
    }

    return 0;
}



/*--------------------------------------------------------------------*
 *                             Safe realloc                           *
 *--------------------------------------------------------------------*/
/*!
 *  reallocNew()
 *
 *      Input:  &indata (<optional>; nulls indata)
 *              size of input data to be copied (bytes)
 *              size of data to be reallocated (bytes)
 *      Return: ptr to new data, or null on error
 *
 *  Action: !N.B. (3) and (4)!
 *      (1) Allocates memory, initialized to 0
 *      (2) Copies as much of the input data as possible
 *          to the new block, truncating the copy if necessary
 *      (3) Frees the input data
 *      (4) Zeroes the input data ptr
 *
 *  Notes:
 *      (1) If newsize <=0, just frees input data and nulls ptr
 *      (2) If input ptr is null, just callocs new memory
 *      (3) This differs from realloc in that it always allocates
 *          new memory (if newsize > 0) and initializes it to 0,
 *          it requires the amount of old data to be copied,
 *          and it takes the address of the input ptr and
 *          nulls the handle.
 */
void *
reallocNew(void   **pindata,
           l_int32  oldsize,
           l_int32  newsize)
{
l_int32  minsize;
void    *indata; 
void    *newdata;

    PROCNAME("reallocNew");

    if (!pindata)
        return ERROR_PTR("input data not defined", procName, NULL);
    indata = *pindata;

    if (newsize <= 0) {   /* nonstandard usage */
        if (indata) {
            FREE(indata);
            *pindata = NULL;
        }
        return NULL;
    }

    if (!indata)   /* nonstandard usage */
    {
        if ((newdata = (void *)CALLOC(1, newsize)) == NULL)
            return ERROR_PTR("newdata not made", procName, NULL);
        return newdata;
    }

        /* Standard usage */
    if ((newdata = (void *)CALLOC(1, newsize)) == NULL)
        return ERROR_PTR("newdata not made", procName, NULL);
    minsize = L_MIN(oldsize, newsize);
    memcpy((char *)newdata, (char *)indata, minsize);

    FREE(indata);
    *pindata = NULL;

    return newdata;
}
    


/*--------------------------------------------------------------------*
 *                       Reading bytes from file                      *
 *--------------------------------------------------------------------*/
/*!
 *  arrayRead()
 *
 *      Input:  filename
 *              &nbytes (<return> number of bytes read)
 *      Return: array, or null on error
 */
l_uint8 *
arrayRead(const char  *fname, 
           l_int32     *pnbytes)
{
l_uint8  *data;
FILE     *fp;

    PROCNAME("arrayRead");

    if (!fname)
        return (l_uint8 *)ERROR_PTR("fname not defined", procName, NULL);
    if (!pnbytes)
        return (l_uint8 *)ERROR_PTR("pnbytes not defined", procName, NULL);
    *pnbytes = 0;

    if ((fp = fopen(fname, "r")) == NULL)
        return (l_uint8 *)ERROR_PTR("file stream not opened", procName, NULL);

    data = arrayReadStream(fp, pnbytes);
    fclose(fp);

    return data;
}


/*!
 *  arrayReadStream()
 *
 *      Input:  stream
 *              &nbytes (<return> number of bytes read)
 *      Return: null-terminated array, or null on error
 *              (reading 0 bytes is not an error)
 *
 *  Notes:
 *      (1) N.B.: as a side effect, this always re-positions the
 *          stream ptr to the beginning of the file.
 */
l_uint8 *
arrayReadStream(FILE     *fp, 
                l_int32  *pnbytes)
{
l_uint8  *data;

    PROCNAME("arrayReadStream");

    if (!fp)
        return (l_uint8 *)ERROR_PTR("stream not defined", procName, NULL);
    if (!pnbytes)
        return (l_uint8 *)ERROR_PTR("ptr to nbytes not defined", procName, NULL);

    *pnbytes = nbytesInFile(fp);

    if ((data = (l_uint8 *)CALLOC(1, *pnbytes + 1)) == NULL)
        return (l_uint8 *)ERROR_PTR("CALLOC fail for data", procName, NULL);
    fread(data, *pnbytes, 1, fp);

    return data;
}


/*!
 *  nbytesInFile()
 *
 *      Input:  file stream
 *      Return: nbytes in file; 0 on error
 */
l_int32
nbytesInFile(FILE  *fp)
{
l_int32  nbytes, pos;

    PROCNAME("nbytesInFile");

    if (!fp)
        return ERROR_INT("stream not open", procName, 0);

    pos = ftell(fp);          /* initial position */
    fseek(fp, 0, SEEK_END);   /* EOF */
    nbytes = ftell(fp);
    fseek(fp, 0, pos);        /* back to initial position */

    return nbytes;
}



/*--------------------------------------------------------------------*
 *                          Writing bytes to file                     *
 *--------------------------------------------------------------------*/
/*!
 *  arrayWrite()
 *
 *      Input:  filename (output)
 *              operation  ("w" for write; "a" for append)
 *              data  (binary data to be written)
 *              nbytes  (size of data array)
 *      Return: 0 if OK; 1 on error
 */
l_int32
arrayWrite(const char  *filename,
           const char  *operation,
           void        *data,
           l_int32      nbytes)
{
FILE  *fp;

    PROCNAME("arrayWrite");

    if (!filename)
        return ERROR_INT("filename not defined", procName, 1);
    if (!operation)
        return ERROR_INT("operation not defined", procName, 1);
    if (!data)
        return ERROR_INT("data not defined", procName, 1);
    if (nbytes <= 0)
        return ERROR_INT("nbytes must be > 0", procName, 1);

    if (!strcmp(operation, "w") && !strcmp(operation, "a"))
        return ERROR_INT("operation not one of {'w','a'}", procName, 1);

    if ((fp = fopen(filename, operation)) == NULL)
        return ERROR_INT("stream not opened", procName, 1);
    fwrite(data, 1, nbytes, fp);
    fclose(fp);

    return 0;
}


/*--------------------------------------------------------------------------*
 *   16 and 32 bit byte-swapping on big endian and little  endian machines  *
 *                                                                          * 
 *   These are typically used for I/O conversions:                          *
 *      (1) endian conversion for data that was read from a file            *
 *      (2) endian conversion on data before it is written to a file        *
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------*
 *                        16-bit byte swapping                        *
 *--------------------------------------------------------------------*/
#ifdef L_BIG_ENDIAN

l_uint16
convertOnBigEnd16(l_uint16  shortin)
{
    return ((shortin << 8) | (shortin >> 8));
}

l_uint16
convertOnLittleEnd16(l_uint16  shortin)
{
    return  shortin;
}

#else     /* L_LITTLE_ENDIAN */

l_uint16
convertOnLittleEnd16(l_uint16  shortin)
{
    return ((shortin << 8) | (shortin >> 8));
}

l_uint16
convertOnBigEnd16(l_uint16  shortin)
{
    return  shortin;
}

#endif  /* L_BIG_ENDIAN */


/*--------------------------------------------------------------------*
 *                        32-bit byte swapping                        *
 *--------------------------------------------------------------------*/
#ifdef L_BIG_ENDIAN

l_uint32
convertOnBigEnd32(l_uint32  wordin)
{
    return ((wordin << 24) | ((wordin << 8) & 0x00ff0000) |
            ((wordin >> 8) & 0x0000ff00) | (wordin >> 24));
}

l_uint32
convertOnLittleEnd32(l_uint32  wordin)
{
    return wordin;
}

#else  /*  L_LITTLE_ENDIAN */

l_uint32
convertOnLittleEnd32(l_uint32  wordin)
{
    return ((wordin << 24) | ((wordin << 8) & 0x00ff0000) |
            ((wordin >> 8) & 0x0000ff00) | (wordin >> 24));
}

l_uint32
convertOnBigEnd32(l_uint32  wordin)
{
    return wordin;
}

#endif  /* L_BIG_ENDIAN */



/*--------------------------------------------------------------------*
 *                         Opening read stream                        *
 *--------------------------------------------------------------------*/
/*!
 *  fopenReadStream()
 *
 *      Input:  filename 
 *      Return: stream or null on error
 */
FILE *
fopenReadStream(const char  *filename)
{
char  *tail;
FILE  *fp;

    PROCNAME("fopenReadStream");

    if (!filename)
        return (FILE *)ERROR_PTR("filename not defined", procName, NULL);

        /* Try input filename */
    if ((fp = fopen(filename, "rb")))
        return fp;

        /* Else, strip directory and try locally */
    splitPathAtDirectory(filename, NULL, &tail);
    if ((fp = fopen(tail, "rb"))) {
        FREE(tail);
        return fp;
    }
    FREE(tail);

    return (FILE *)ERROR_PTR("file not found", procName, NULL);
}


/*--------------------------------------------------------------------*
 *                         File name operations                       *
 *--------------------------------------------------------------------*/
/*!
 *  splitPathAtDirectory()
 *
 *      Input:  pathname  (full path; can be a directory)
 *              &dir  (<optional return> root directory name of
 *                     input path, including trailing '/')
 *              &tail (<optional return> path tail, which is either
 *                     the file name within the root directory or
 *                     the last sub-directory in the path)
 *      Return: 0 if OK, 1 on error
 *       
 *  Note: (1) if you only want the tail, input null for
 *            the root directory ptr.
 *        (2) if you only want the root directory name,
 *            input null for the tail ptr.
 *        (3) This function makes decisions based only on the lexical
 *            structure of the input.  Examples:
 *              /usr/tmp/abc  -->  dir: /usr/tmp/   tail: abc
 *              /usr/tmp/  -->  dir: /usr/tmp/   tail: [empty string]
 *              /usr/tmp  -->  dir: /usr/   tail: tmp
 */
l_int32
splitPathAtDirectory(const char  *pathname,
                     char       **pdir,
                     char       **ptail)
{
char  *cpathname, *lastslash;

    PROCNAME("splitPathAtDirectory");

    if (!pathname)
        return ERROR_INT("pathname not defined", procName, 1);
    if (!pdir && !ptail)
        return ERROR_INT("null input for both strings", procName, 1);

    cpathname = stringNew(pathname);
    if ((lastslash = strrchr(cpathname, '/'))) {
        if (ptail)
            *ptail = stringNew(lastslash + 1);
        if (pdir) {
            *(lastslash + 1) = '\0';
            *pdir = cpathname;
        }
        else
            FREE(cpathname);
    }
    else {  /* no directory */
        if (pdir)
            *pdir = stringNew("");
        if (ptail)
            *ptail = cpathname;
        else
            FREE(cpathname);
    }

    return 0;
}


/*!
 *  splitPathAtExtension()
 *
 *      Input:  pathname (full path; can be a directory)
 *              &basename (<optional return> pathname not including the
 *                        last dot and characters after that)
 *              &extension (<optional return> path extension, which is
 *                        the last dot and the characters after it.  If
 *                        there is no extension, it returns the empty string)
 *      Return: 0 if OK, 1 on error
 *       
 *  Notes:
 *      (1) If you only want the extension, input null for the basename ptr.
 *      (2) If you only want the basename without extension, input null
 *          for the extension ptr.
 *      (3) This function makes decisions based only on the lexical
 *          structure of the input.  Examples:
 *            /usr/tmp/abc.jpg  -->  basename: /usr/tmp/abc   ext: .jpg
 *            /usr/tmp/.jpg  -->  basename: /usr/tmp/   tail: .jpg
 *            /usr/tmp.jpg/  -->  basename: /usr/tmp.jpg/   tail: [empty str]
 *            ./.jpg  -->  basename: ./   tail: .jpg
 */
l_int32
splitPathAtExtension(const char  *pathname,
                     char       **pbasename,
                     char       **pextension)
{
char  *tail, *dir, *lastdot;
char   empty[4] = "";

    PROCNAME("splitPathExtension");

    if (!pathname)
        return ERROR_INT("pathname not defined", procName, 1);
    if (!pbasename && !pextension)
        return ERROR_INT("null input for both strings", procName, 1);

        /* Split out the directory first */
    splitPathAtDirectory(pathname, &dir, &tail);

        /* Then look for a "." in the tail part.
         * This way we ignore all "." in the directory. */
    if ((lastdot = strrchr(tail, '.'))) {
        if (pextension)
            *pextension = stringNew(lastdot);
        if (pbasename) {
            *lastdot = '\0';
            *pbasename = stringJoin(dir, tail);
        }
    }
    else {
        if (pextension)
            *pextension = stringNew(empty);
        if (pbasename)
            *pbasename = stringNew(pathname);
    }
    FREE(dir);
    FREE(tail);
    return 0;
}


/*! 
 *  genPathname()
 *
 *      Input:  dir (directory name, with or without trailing '/')
 *              fname (file name within the directory)
 *      Return: full pathname, or null on error
 */
char *
genPathname(const char  *dir,
            const char  *fname)
{
l_int32  dirlen, namelen;
char    *charbuf;
    
    PROCNAME("genPathname");

    if (!dir)
        return (char *)ERROR_PTR("dir not defined", procName, NULL);
    if (!fname)
        return (char *)ERROR_PTR("fname not defined", procName, NULL);

    dirlen = strlen(dir);
    namelen = strlen(fname);
    if ((charbuf = (char *)CALLOC(dirlen + namelen + 10, sizeof(char)))
            == NULL)
        return (char *)ERROR_PTR("charbuf not made", procName, NULL);

    if (dir[dirlen - 1] != '/')
        sprintf(charbuf, "%s/", dir);
    else
        strcpy(charbuf, dir);
    strcat(charbuf, fname);
    return charbuf;
}


/*---------------------------------------------------------------------*
 *                           Timing procs                              *
 *---------------------------------------------------------------------*/
/*
 *  Example of use:
 *
 *      startTimer();
 *      ....
 *      fprintf(stderr, "Elapsed time = %7.3f sec\n", stopTimer());
 */
#if !defined(__MINGW32__) && !defined(_WIN32)

#include <sys/time.h>
#include <sys/resource.h>

static struct rusage rusage_before;
static struct rusage rusage_after;

void
startTimer(void)
{
    getrusage(RUSAGE_SELF, &rusage_before);
}


l_float32
stopTimer(void)
{
l_int32  tsec, tusec;

    getrusage(RUSAGE_SELF, &rusage_after);

    tsec = rusage_after.ru_utime.tv_sec - rusage_before.ru_utime.tv_sec;
    tusec = rusage_after.ru_utime.tv_usec - rusage_before.ru_utime.tv_usec;

    return (tsec + ((l_float32)tusec) / 1000000.0);
}

#else   /* __MINGW32__ : resource.h not implemented under MINGW */

#include <windows.h>

static ULARGE_INTEGER utime_before;
static ULARGE_INTEGER utime_after;

void
startTimer(void)
{
HANDLE    this_process;
FILETIME  start, stop, kernel, user;
    
    this_process = GetCurrentProcess ();

    GetProcessTimes (this_process, &start, &stop, &kernel, &user);

    utime_before.LowPart  = user.dwLowDateTime;
    utime_before.HighPart = user.dwHighDateTime;
}

l_float32
stopTimer(void)
{
HANDLE    this_process;
FILETIME  start, stop, kernel, user;
    
    this_process = GetCurrentProcess ();

    GetProcessTimes (this_process, &start, &stop, &kernel, &user);

    utime_after.LowPart  = user.dwLowDateTime;
    utime_after.HighPart = user.dwHighDateTime;

    return ((l_float32)(signed)(utime_after.QuadPart - utime_before.QuadPart) /
             10000000.0);
}

#endif

