/*!
 * FileName: CharDetect.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-8-28
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 文档编码判断类
 */


#ifndef _CHAR_DETECT_H_
#define _CHAR_DETECT_H_
#include <QObject>
#include <QString>

#include "CharGlobal.h"
#include "uchardet/uchardet.h"

namespace chardet
{
	struct FileBomInfo 
	{
		bool has_bom; //是否有bom
		QString endian;  //大小端 "Little Endian" | "Big Endian" | ""
		QByteArray bom_header; //bom原始头
	};

	class TextCodecInfo
	{
	public:
		TextCodecInfo()
		{
			state_type = 0;
			codec = "";
			extend_info = "";
			bom_info.has_bom = false;
			bom_info.endian = "";
			bom_info.bom_header = QByteArray();
		}

		TextCodecInfo(const QString &_codec, bool _has_bom = false, QString &_endian = QString(), const QByteArray &_bom_header = QByteArray())
		{
			state_type = 0;
			codec = "";
			extend_info = "";
			bom_info.has_bom = false;
			bom_info.endian = "";
			bom_info.bom_header = QByteArray();
			if (!_codec.isEmpty())
			{
				state_type = 1;
				codec = _codec;
				bom_info.has_bom = _has_bom;
				bom_info.endian = _endian;
				bom_info.bom_header = _bom_header;
			}
		}
	public:
		int state_type;  //信息状态 -1错误信息 0空信息 1正常编码信息
		QString codec;   //编码类型字符串
		QString extend_info;  //扩展信息 一般用于错误信息
		FileBomInfo bom_info; //文件BOM信息
	};

	class CharDetect :public QObject
	{
		Q_OBJECT
	public:
		CharDetect(QObject* parent = NULL);
		virtual ~CharDetect();


		//************************************
		// Method:    charDetect
		// FullName:  uchardet::CharDetect::charDetect
		// Access:    public static 
		// Returns:   QT_NAMESPACE::QString 
		// Qualifier: 判断文本文档编码格式 并返回
		// Parameter: const QString file_path 文件路径
		// Parameter: size_t section_size 每回读取文件的字节数
		//************************************
		static TextCodecInfo charDetect(const QString file_path, size_t section_size = 4096, int max_sections = INT_MAX);

	private:
		static FileBomInfo checkBomHeader(const char header_buff[], size_t size);
	};

	
}//end uchardet ns


Q_DECLARE_METATYPE(chardet::TextCodecInfo)

#endif