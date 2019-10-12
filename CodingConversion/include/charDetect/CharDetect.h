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

	struct TextCodecInfo 
	{
		QString codec;
		FileBomInfo bom_info;
	};

	class CharDetect :public QObject
	{
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

