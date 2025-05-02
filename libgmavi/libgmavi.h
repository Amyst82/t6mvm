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

#ifndef LIBGMAVI_H
# define LIBGMAVI_H
# include <stdint.h>
# include <stdbool.h>
# ifdef __cplusplus
extern "C" {
# endif

	/*
	*	Open a (new) file ready to receive frame data
	*
	*	@param	filePath		- Full path or name suffixed with the ".avi" extension
	*	@param	width			- Width of the video
	*	@param	height			- Height of the video
	*	@param	framesPerSec	- Frames per second
	*	@return gmavi instance (void *)
	*/
	void* gmav_open(const char* filePath, uint32_t width, uint32_t height, uint32_t framesPerSec);

	/*
	*	Add a frame to the current file stream
	*
	*	@param	gmavi			- gmavi instance
	*	@param	buffer			- 24bits per pixel bitmap array (bottom first)
	*/
	bool		gmav_add(void* gmavi, uint8_t* buffer);

	/*
	*	Finish and close file
	*
	*	@param	gmavi			- gmavi instance
	*/
	bool		gmav_finish(void* gmavi);

# ifdef __cplusplus
}
# endif
#endif
