#pragma once
#ifndef INFOBOARD_H
#define INFOBOARD_H
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QApplication>
#include <iostream>
#include <queue>
#include <mutex>

class infoBoard :public QObject
{
	Q_OBJECT
public:
	infoBoard(QTextEdit* textEdit):ptrInfoTextEdit(textEdit)
	{
		//textEdit设置为只读
		ptrInfoTextEdit->setReadOnly(true);
		// 设置不可选中
		ptrInfoTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
	}
	~infoBoard()
	{
	}
	infoBoard(const infoBoard&) = delete;
	infoBoard& operator=(const infoBoard&) = delete;

	void append(const std::string info)
	{
		std::lock_guard<std::mutex> lock(mut);
		infoQue.emplace(info);
	}
private:
	QTextEdit* ptrInfoTextEdit;
	std::mutex mut;
	std::queue<std::string> infoQue;

	bool getFront(std::string & s)
	{
		
		std::lock_guard<std::mutex> lock(mut);
		if (infoQue.empty())
		{
			return false;
		}
		s = infoQue.front();
		infoQue.pop();
		return true;
	}

signals:
	void waitPrint();
public slots:
	void printInfo()
	{
		std::string info = ">";
		std::string tempInfo;
		while (getFront(tempInfo))
		{
			info = info + tempInfo;
		}
		ptrInfoTextEdit->insertPlainText(info.c_str());
		ptrInfoTextEdit->moveCursor(QTextCursor::End);
	}
};


#endif
