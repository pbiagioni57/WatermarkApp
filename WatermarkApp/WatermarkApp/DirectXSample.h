/****************************** Module Header ******************************\
* Module Name:  DirectXSample.h
* Project:      WatermarkComponent
* Copyright (c) Microsoft Corporation.
*
* This header defines helper utilities to make DirectX APIs work with exceptions.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once
#include <intsafe.h>
#include <comdef.h>
#include "CustomError.h"

// This header defines helper utilities to make DirectX APIs work with exceptions.
namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw hr;
		}
	}
	inline void ThrowIfFailed(HRESULT hr, const char * msg)
	{
		if (FAILED(hr))
		{
			// todo: simplify string conversion
			_com_error err(hr);
			const wchar_t * wErrMsg = err.ErrorMessage();
			char* errMsg = new char[1024];
			wcstombs(errMsg, wErrMsg, 1024);
			char outMsg[1024];
			sprintf(outMsg, "%s: %s", msg, errMsg);
			throw CustomError(outMsg);
		}
	}
}
