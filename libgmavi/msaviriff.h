//------------------------------------------------------------------------------
// File: AVIRIFF.h
//
// Desc: Structures and defines for the RIFF AVI file format extended to
//       handle very large/long files.
//
// Copyright (c) 1996-2001, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef MSAVIRIFF_H
# define MSAVIRIFF_H
# define DWORD  unsigned int
#pragma warning(disable: 4097 4511 4512 4514 4705)
#pragma warning(disable: 4996)
#pragma warning(disable: 4200)
# define FCC(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))

#endif
