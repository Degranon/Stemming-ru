#include <QTest>
#include "RStemmerTest.h"

RStemmerTest::RStemmerTest(QObject *parent) :
	QObject(parent)
{
}

void RStemmerTest::FileAnalyserWordCountTest_data()
{
	QTest::addColumn<string>("FileName");
	QTest::addColumn<int>("WordCount"); 
	QTest::newRow("words") << (string) "Test_data/words.txt" << 11;
	QTest::newRow("words_and_empty_lines") << (string) "Test_data/words_and_empty_lines.txt" << 11;
	QTest::newRow("empty") << (string) "Test_data/empty.txt" << 0;
	QTest::newRow("whitespace_chars") << (string) "Test_data/whitespace_chars.txt" << 0;
}

void RStemmerTest::FileAnalyserWordCountTest()
{
	QFETCH(string, FileName);
	QFETCH(int, WordCount);
	try {		
		FileAnalyser fa(FileName);
		QCOMPARE((int)fa.getWordNo(), WordCount);
	}
	catch (std::exception &ex) {
		std::cout << "Exception occured: " << ex.what() << std::endl;
		QVERIFY(false);
	}	
	
}

void RStemmerTest::StemmerPorterFunctionalityTest_data()
{
	QTest::addColumn<string>("Word");
	QTest::addColumn<string>("Stem");
	QTest::newRow("word_1") << string("���������") << string("�������");
	QTest::newRow("word_2") << string("����������������") << string("����������");
	QTest::newRow("word_3") << string("�����") << string("���");
	QTest::newRow("word_4") << string("����������") << string("����");
	QTest::newRow("word_5") << string("�����") << string("���");
	QTest::newRow("word_6") << string("��������") << string("������");
	QTest::newRow("word_7") << string("����������") << string("������");
	QTest::newRow("word_8") << string("�������") << string("����");
	QTest::newRow("word_9") << string("����������") << string("��������");
	QTest::newRow("word_10") << string("���������") << string("�������");
	QTest::newRow("word_11") << string("�������") << string("���");
	QTest::newRow("word_12") << string("����") << string("��");
}


void RStemmerTest::StemmerPorterFunctionalityTest()
{
	QFETCH(string, Word);
	QFETCH(string, Stem);
	StemmerPorter sp;
	
	QCOMPARE(sp.get(Word), Stem);
}