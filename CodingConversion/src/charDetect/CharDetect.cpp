/*!
 * FileName: CharDetect.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-8-28
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 文档编码判断类
 */

#include <QFile>
#include <QDebug>

#include "CharGlobal.h"
#include "charDetect/CharDetect.h"


namespace chardet
{
	CharDetect::CharDetect(QObject* parent /*= NULL*/) :QObject(parent)
	{

	}

	CharDetect::~CharDetect()
	{

	}

	TextCodecInfo CharDetect::charDetect(const QString file_path, size_t section_size/*= 4096*/, int max_sections/*= INT_MAX*/)
	{
		TextCodecInfo res_codec;

		FILE* fp = NULL;
		uchardet_t handle = uchardet_new();
		const char* charset;
		char* buffer = new char[section_size];
		int now_sections = 0;

		if (buffer == NULL)
		{
			qDebug() << "No Memary";
			res_codec.state_type = -1;
			res_codec.extend_info = trUtf8("探测异常");
			return res_codec;
		}

		if (!QFile::exists(file_path))
		{
			qDebug() << "File " << file_path << " not exists";
			res_codec.state_type = -1;
			res_codec.extend_info = trUtf8("文件丢失");
			return res_codec;
		}

		fp = fopen(file_path.toStdString().c_str(), "r");
		if (fp == NULL)
		{
			qDebug() << "open " << file_path << "Error";
			res_codec.state_type = -1;
			res_codec.extend_info = trUtf8("打开异常");
			return res_codec;
		}

		while (now_sections <= max_sections && !feof(fp))
		{
			size_t len = fread(buffer, 1, section_size, fp);

			if (now_sections == 0)
			{
				res_codec.bom_info = checkBomHeader(buffer, len);
			}

			int retval = uchardet_handle_data(handle, buffer, len);
			++now_sections;
			if (retval != 0)
			{
				break;
			}
		}

		delete[] buffer;
		fclose(fp);

		uchardet_data_end(handle);

		charset = uchardet_get_charset(handle);
		if (charset != NULL)
		{
			res_codec.state_type = 1;
			res_codec.codec = trUtf8(charset);
		}
		else
		{
			res_codec.state_type = 1;
			res_codec.codec = trUtf8("未知编码");
		}

		uchardet_delete(handle);

		return res_codec;
	}

	FileBomInfo CharDetect::checkBomHeader(const char header_buff[], size_t size)
	{
		FileBomInfo res;
		res.has_bom = false;
		res.endian = "";
		res.bom_header.clear();

		if (size > 2)
		{
			switch (header_buff[0])
			{
			case '\xEF':
				if (('\xBB' == header_buff[1]) && ('\xBF' == header_buff[2]))
				{
					/* EF BB BF: UTF-8 encoded BOM. */
					res.has_bom = true;
					res.endian = "";
					res.bom_header.append(header_buff, 3);
				}
				break;
			case '\xFE':
				if ('\xFF' == header_buff[1])
				{
					/* FE FF: UTF-16, big endian BOM. */
					res.has_bom = true;
					res.endian = "Big Endian";
					res.bom_header.append(header_buff, 2);
				}
				break;
			case '\xFF':
				if ('\xFE' == header_buff[1])
				{
					if (size > 3 && header_buff[2] == '\x00' && header_buff[3] == '\x00')
					{
						/* FF FE 00 00: UTF-32 (LE). */
						res.has_bom = true;
						res.endian = "Little Endian";
						res.bom_header.append(header_buff, 4);
					}
					else
					{
						/* FF FE: UTF-16, little endian BOM. */
						res.has_bom = true;
						res.endian = "Little Endian";
						res.bom_header.append(header_buff, 2);
					}
				}
				break;
			case '\x00':
				if (size > 3 && header_buff[1] == '\x00' && header_buff[2] == '\xFE' && header_buff[3] == '\xFF')
				{
					/* 00 00 FE FF: UTF-32 (BE). */
					res.has_bom = true;
					res.endian = "Big Endian";
					res.bom_header.append(header_buff, 4);
				}
				break;
			}
		}
		return res;
	}

}//end uchardet ns

