/*
*	Copyright (c) 2022 Gijs Oosterling
*	All rights reserved.
*	
*		Permission is hereby granted, free of charge, to any person obtaining a copy
*		of this software and associated documentation files (the "Software"), to deal
*		in the Software without restriction, including without limitation the rights
*		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*		copies of the Software, and to permit persons to whom the Software is
*		furnished to do so, subject to the following conditions:
*	
*		The above copyright notice and this permission notice shall be included in all
*		copies or substantial portions of the Software.
*	
*		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*		SOFTWARE.
*	
*	Redistributions in binary form must reproduce the above copyright notice.
*/

#ifndef AVISTRUCT_H
# define AVISTRUCT_H
# include "..\..\..\..\..\..\..\Mac\Home\Desktop\libgmavi-main\src\msaviriff.h"
# include <pshpack2.h>
# include <stdint.h>
# include <limits.h>

# define TO_BE_DETERMINED				0x0
# define RIFF_MAX_SIZE					1999991696
# define STATIC_HEADER_LIST_SIZE		4588
# define STATIC_AVI_HEADER_SIZE			56
# define STATIC_STREAM_LIST_SIZE		4244
# define STATIC_STREAM_HEADER_SIZE		56
# define STATIC_STREAM_FORMAT_SIZE		40
# define STATIC_BITMAP_HEADER_SIZE		40
# define STATIC_SUPER_INDEX_SIZE		4120
# define STATIC_EXTENDED_LIST_SIZE		260
# define STATIC_EXTENDED_HEADER_SIZE	248
# define STATIC_JUNKFILL_SIZE			0xDF8
# define STATIC_SUPER_INDEX_OFFSET		0x10
# define STATIC_OLD_INDEX_OFFSET		0x10

/*	Size limitation for RIFFLISTs */
# define	AVI_MAX_RIFF_SIZE		0x40000000LL
# define	AVI_MASTER_INDEX_SIZE   256
# define	AVIIF_INDEX				0x10
# define	FOURCC					unsigned int
/*
**	Reduce uint8_t padding to one for precise data alignment
*/
#pragma pack(push,1)

/*
*	RIFFCHUNK
*
*	@param		fcc 					-	Describes the fcc ID of a chunk
*	@param		cb						-	Size of chunk (not including this struct)
*/
typedef struct	_riffChunk
{
	FOURCC		fcc;
	uint32_t	cb;
}	RIFFCHUNK;

# define	FOURCC_RIFF				FCC('RIFF')
# define	FOURCC_LIST				FCC('LIST')
# define	FOURCC_TYPE_VIDEO		FCC('AVI ')
# define	FOURCC_TYPE_VIDEO_EXT	FCC('AVIX')
/*
*	RIFFLIST
*
*	@param		fcc 					-	Describes the fcc ID of a list
*	@param		cb						-	Size of list data (not including @fcc and @cb of this struct)
*	@param		fccListType				-	List Type
*/
typedef struct	_riffList
{
	FOURCC		fcc;
	uint32_t	cb;
	FOURCC		fccListType;
}	RIFFLIST;

# define	AVIF_HASINDEX			0x00000010
# define	AVIF_MUSTUSEINDEX   	0x00000020
# define	AVIF_ISINTERLEAVED		0x00000100
# define	AVIF_TRUSTCKTYPE		0x00000800
# define	AVIF_WASCAPTUREFILE		0x00010000
# define	AVIF_COPYRIGHTED		0x00020000

/*
*	AVIMAINHEADER
*
*	@param		fcc						-	FOURCC code 'avih'	@FOURCC_AVI_HEADER
*	@param		cb						-	Size of this structure (minus @fcc and @cb)
*	@param		microSecPerFrame		-	Microseconds per frame (1000000 / frameRateSec)
*	@param		maxBytesPerSec			-	Approximate maximum data rate (W*H*FMT*FRAMES?)
*	@param		paddingGranularity		-	Data alignment. Multiples of this value
*	@param		flags					-	Metadata flags
*	@param		totalFrames				-	Amount of frames in the file		(*)
*	@param		initialFrames			-	Initial frame for interleaved files, otherwise zero
*	@param		streams					-	Amount of streams in this file (one for video and one for audio)
*	@param		suggestedBufferSize		-	Buffer size for reading frames (W*H*FMT*FRAMES?)
*	@param		width					-	Width of the fideo buffer
*	@param		height					-	Height of the video buffer
*	@param		reserved				-	Unused, should be NULL (zeroed)
*/
typedef struct 	_aviMainHeader
{
	FOURCC		fcc;
	uint32_t	cb;
	uint32_t	microSecPerFrame;
	uint32_t	maxBytesPerSec;
	uint32_t	paddingGranularity;
	uint32_t	flags;
	uint32_t	totalFrames;
	uint32_t	initialFames;
	uint32_t	streams;
	uint32_t	suggestedBufferSize;
	uint32_t	width;
	uint32_t	height;
	uint32_t	reserved[4];
}	AVIMAINHEADER;

/*
*	RECT
*
*	rectangle definition, dimensions to screen.
*/

typedef struct	_rect
{
	short int	left;
	short int	top;
	short int	right;
	short int	bottom;
}	RECT;

# define	AVISF_DISABLED				0x00000001
# define 	AVISF_VIDEO_PALCHANGES   	0x00010000

/*
*	AVISTREAMHEADER
*
*	@param		fcc						-	FOURCC code 'strh'	@FOURCC_STREAM_HEADER
*	@param		cb						-	Size of this structure (minus @fcc and @cb)
*	@param		fccType					-	FOURCC code specifying the type of data contained in the stream
*	@param		fccHandler				-	FOURCC code describing the codec used ('DIB ' for Bitstream)	@FOURCC_HANDLER_DIB
*	@param		flags					-	Flags for data stream
*	@param		priority				-	Priority for stream type
*	@param		language				-	Language tag
*	@param		initialFrames			-	Initial frame for interleaved files, otherwise zero
*	@param		scale					-	Frame scale (1)
*	@param		rate					-	Frame rate	(Frames per second)
*	@param		start					-	zero
*	@param		length					-	Frame count
*	@param		suggestedBufferSize		-	(W*H*FMT + 8)
*	@param		quality					-	0xFFFFFFFF (-1 when uncompressed)
*	@param		frame					-	Destination rectangle for cropping
*/
typedef struct	_aviStreamHeader
{
	FOURCC			fcc;
	uint32_t		cb;
	FOURCC			fccType;
	FOURCC			fccHandler;
	uint32_t		flags;
	uint16_t		priority;
	uint16_t		language;
	uint32_t		initialFrames;
	uint32_t		scale;
	uint32_t		rate;
	uint32_t		start;
	uint32_t		length;
	uint32_t		suggestedBufferSize;
	uint32_t		quality;
	uint32_t		sampleSize;
	RECT			frame;
}	AVISTREAMHEADER;

/*
*	BITMAPINFOHEADER
*
*	Standard bitmap header (DIB)
*/
typedef struct	_bitMapInfoHeader
{
	uint32_t		size;
	uint32_t		width;
	uint32_t		height;
	uint16_t		planes;
	uint16_t		bitCount;
	uint32_t		compression;
	uint32_t		sizeImage;
	uint32_t		xPelsPerMeter;
	uint32_t		yPelsPerMeter;
	uint32_t		clrUsed;
	uint32_t		clrImportant;
}	BITMAPINFOHEADER;

/*
*	AVISUPERINDEX_ENTRY
*
*	@param		offset			-	64 bit offset to field index chunk (@AVISTDINDEX)
*	@param		size			-	size of chunk
*	@param		duration		-	duration of chunk (in stream ticks/frames)
*/
typedef struct	_aviSuperIndex_entry
{
	uint64_t	offset;
	uint32_t	size;
	uint32_t	duration;
}	AVISUPERINDEX_ENTRY;

# define	AVI_INDEX_OF_INDEXES	0x00
# define	AVI_INDEX_OF_CHUNKS		0x01
/*
*	AVISUPERINDEX
*
*	@param				fcc				-	FOURCC identifier 'indx'	@FOURCC_INDEX_DATA
*	@param				cb				-	Size of this struct minus @fcc and @cb
*	@param				longsPerEntry	-	Must be 4
*	@param				indexSubType	-	0
*	@param				indexType		-	@AVI_INDEX_OF_INDEXES
*	@param				entriesInUse	-	?
*	@param				chunkId			-	Chunk ID of chunks being indexed ('DIB '?)
*	@param				reserved		-	Reserved 12 Bytes
*	@param				index			-	Index entries pointing to field index chunk (@AVISTDINDEX)
*/
typedef struct	_aviSuperIndex
{
	FOURCC				fcc;
	uint32_t			cb;
	uint16_t			longsPerEntry;
	uint8_t				indexSubType;
	uint8_t				indexType;
	uint32_t			entriesInUse;
	uint32_t			chunkId;
	uint32_t			reserved[3];
	AVISUPERINDEX_ENTRY	index[AVI_MASTER_INDEX_SIZE];
}	AVISUPERINDEX;

/*
*	AVIEXTHEADER
*
*	@param				fcc				-	FOURCC identifier 'dmlh'	@FOURCC_EXTENDED_HEADER
*	@param				cb				-	Size of this struct minus @fcc and @cb
*	@param				grandFrames		-	Total number of frames in the file
*	@param				future			-	undefined
*/
typedef struct	_aviExtHeader
{
	FOURCC				fcc;
	uint32_t			cb;
	uint32_t			grandFrames;
	uint32_t			future[61];
}	AVIEXTHEADER;

/*
*	RGB24BITMAP
*
*	@param	fcc				-	FOURCC format identifier '00db'	@FOURCC_STREAM_RAW
*	@param	cb				-	size minus @fcc and @cb
*	@param	bitmap			-	Bitmap with size of (W * H * FMT eg. 1280*720*3)
* */
typedef struct	_rgb24Bitmap
{
	uint32_t			fcc;
	uint32_t			cb;
	uint8_t				*bitmap;
}	RGB24BITMAP;

/*
*	OLD INDEX
*/
typedef struct	_aviOldIndex_entry
{
	uint32_t			chunkId;
	uint32_t			flags;
	uint32_t			offset;
	uint32_t			size;
}	AVIOLDINDEX_ENTRY;

typedef struct	_aviOldIndex
{
	FOURCC				fcc;
	uint32_t			cb;
}	AVIOLDINDEX;

/*
*	AVISTDINDEX_ENTRY
*
*	@param		offset				-	offset to bitmap (from base offset)
*	@param		cb					-	size of the bitmap information
*/
typedef struct _avistdindex_entry 
{
	uint32_t dwOffset;
	uint32_t dwSize;
}   AVISTDINDEX_ENTRY;

# define 	AVI_INDEX_2FIELD 			0x01
# define 	AVI_INDEX_OF_CHUNKS 		0x01

/*
*	AVISTDINDEX
*
*	@param				fcc				-	FOURCC identifier 'ix00'	@FOURCC_TYPE_FIELD_INDEX
*	@param				cb				-	Size of this struct minus @fcc and @cb
*	@param				longsPerEntry	-	3
*	@param				indexSubType	-	@AVI_INDEX_2FIELD
*	@param				indexType		-	@AVI_INDEX_OF_CHUNKS
*	@param				entriesInUse	-	?
*	@param				chunkId			-	'00db'
*	@param				baseOffset		-	offsets in field index entries are relative to this
*	@param				reserved3		-	0
*	@param				index[ ];		-	field index entries		@AVISTDINDEX_ENTRY
*/
typedef struct _avistdindex
{
	FOURCC				fcc;
	uint32_t			cb;
	uint16_t			wLongsPerEntry;
	uint8_t				bIndexSubType;
	uint8_t				bIndexType;
	uint32_t			nEntriesInUse;
	uint32_t			dwChunkId;
	uint64_t			qwBaseOffset;
	uint32_t			dwReserved_3;
}   AVISTDINDEX;

# define	FOURCC_JUNK		FCC('JUNK')

/*
*	File address reference
*/
typedef struct s_gmavi_fileAddr
{
	uint64_t			_fileBaseStart;
	uint64_t			cbMain;
	uint64_t			firstFrames;	//until 2gb
	uint64_t			totalFrames;
	uint64_t			superIndex;
	uint64_t			entriesInUse;
	uint64_t			superIndexEntries;
	uint64_t			grandFrames;
	uint64_t			cbMovi;
	uint64_t			moviStart;
}   gmavi_fileAddr_t;

/*
*	Initial header that can be written upon opening	
*/
typedef struct	s_gmavi_static
{
	RIFFLIST			main;
	RIFFLIST			hdrl;
	AVIMAINHEADER		aviHeader;
	RIFFLIST			strl;
	AVISTREAMHEADER		streamHeader;
	RIFFCHUNK			strf;
	BITMAPINFOHEADER	bitmapHeader;
	AVISUPERINDEX		superIndex;
	RIFFLIST			odml;
	AVIEXTHEADER		extendedHeader;
	RIFFCHUNK			junk;
	uint8_t				junkFill[0xDF8];
	RIFFLIST			movi;
}   gmavi_static_t;


typedef struct	s_idxList
{
	AVISTDINDEX			avixIndex;
	AVISTDINDEX_ENTRY	*avixIndexEntries;
}	t_idxList;

/*
*	Avi handler struct, do not modify
*/
typedef struct	s_gmavi
{
	char				*filePath;
	FILE				*fileHandler;
	gmavi_static_t		contents;
	gmavi_fileAddr_t	fileAddr;
	AVIOLDINDEX			mainIndex;
	AVIOLDINDEX_ENTRY	*mainIndexEntries;
	t_idxList			ix00[256];
	uint32_t			frameCount;
	uint32_t			bitmapSize;
	uint32_t			streamTickSize;
	uint32_t			riffSize;
	uint64_t			fileSize;
	uint32_t			moviSize;
	uint32_t			maxFrames;
	uint32_t			riffChunks;
}	gmavi_t;

/*	Only pack these structs		*/
#pragma pack(pop)

#endif
