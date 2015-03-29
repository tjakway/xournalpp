/*
 * Xournal++
 *
 * Part of the PDF export
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv3
 */

#ifndef __PDFWRITER_H__
#define __PDFWRITER_H__

#include <gtk/gtk.h>
#include <StringUtils.h>
#include "PdfXRef.h"
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
using boost::filesystem::path;

class PdfWriter
{
public:
	PdfWriter(PdfXRef* xref);
	virtual ~PdfWriter();

public:
	void close();
	bool openFile(path filename);

public:
	bool write(string data);
	bool write(boost::format data);
	bool writeTxt(string data);
	bool write(int data);

	void startStream();
	void endStream();

	bool writeInfo(string title);
	bool writeObj();


	string getLastError();

	int getObjectId();
	int getNextObjectId();
	int getDataCount();

public:
	static void setCompressPdfOutput(bool compress);

private:
	XOJ_TYPE_ATTRIB;

	static bool compressPdfOutput;

	bool inStream;
	string stream;

	GFileOutputStream* out;

	string lastError;

	PdfXRef* xref;

	int objectId;

};

#endif /* __PDFWRITER_H__ */
