#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	loader = new Loader();
	memory = new MemoryManager();
	nixbpe = new NIXBPE();
	registers = new Registers();
	executor = NULL;
	OpenFileState = false;
	ProgramState = false;

	ui->RegsPCContentLable->setText("000000");
	ui->RegsAContentLable->setText("000000");
	ui->RegsXContentLable->setText("000000");
	ui->RegsLContentLable->setText("FFFFFF");
	ui->RegsBContentLable->setText("000000");
	ui->RegsSContentLable->setText("000000");
	ui->RegsTContentLable->setText("000000");
	ui->RegsSWContentLable->setText("000000");
	ui->LoadAddrLineEdit->setText("000000");
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_SelectFileBtn_clicked() {
	// clear re open file
	ui->CurInstructContentLable->setText("");
	ui->RegsPCContentLable->setText("000000");
	ui->RegsAContentLable->setText("000000");
	ui->RegsXContentLable->setText("000000");
	ui->RegsLContentLable->setText("FFFFFF");
	ui->RegsBContentLable->setText("000000");
	ui->RegsSContentLable->setText("000000");
	ui->RegsTContentLable->setText("000000");
	ui->RegsSWContentLable->setText("000000");

	// enable input loading address
	ui->LoadAddrLineEdit->setDisabled(false);

	ui->MemeryDisplayPlainTextEdit->setPlainText("");
	ui->OpenFileStatusLable->setStyleSheet(QString("background-image: url(:/Image/FileNotSelected.png);"));
	ui->OpenFileTextEdit->setText("");

	// setWindow Title
	this->setWindowTitle(QString("單步執行器"));

	delete loader;
	delete memory;
	delete nixbpe;
	delete registers;

	loader = new Loader();
	memory = new MemoryManager();
	nixbpe = new NIXBPE();
	registers = new Registers();
	OpenFileState = false;
	ProgramState = false;

	QString FilePathName;
	string fileName;
	FilePathName = QFileDialog::getOpenFileName(this, "選擇開啟的檔案...");
	if (!FilePathName.isEmpty()) {
		ui->OpenFileTextEdit->setText(FilePathName);
		fileName= FilePathName.toLocal8Bit().toStdString();

		// process loadAddrStr from LoadAddrTextEdit insure input in 0-9, A-F, a-f
		QString loadAddrStr = ui->LoadAddrLineEdit->text();

		if (this->validator(loadAddrStr)) {
			// disable input loading address
			ui->LoadAddrLineEdit->setDisabled(true);
		} else {
			ui->LoadAddrLineEdit->setText("000000");
			ui->OpenFileTextEdit->setText("");
			popupWindows(QString("載入位置請輸入16進制碼，只限數字與A-F, a-f..."));
			return;
		}

		if (!this->loader->LoadObjectProgram(fileName, loadAddrStr.toStdString())) {
			ui->OpenFileStatusLable->setStyleSheet(QString("background-image: url(:/Image/FileOpenFail.png);"));
			string outMsg = "未能開啟檔案 " + FilePathName.toStdString();
			// enable input loading address
			ui->LoadAddrLineEdit->setDisabled(false);
			popupWindows(QString::fromStdString(outMsg));
			return;
		}

		// setWindow Title
		string stdWindowTitle = "單步執行器--" + loader->GetHeadRecord()->name;
		QString newWindowTitle = QString::fromStdString(stdWindowTitle);
		this->setWindowTitle(newWindowTitle);

		// be sure the file has opened
		OpenFileState = true;

		ui->OpenFileStatusLable->setStyleSheet(QString("background-image: url(:/Image/FileOpenSucc.png);"));
		this->memory->LoadInMemory(this->loader);

		string stdStr = this->memory->Output();
		QString toMemBlockLabel = QString::fromStdString(stdStr);
		ui->MemeryDisplayPlainTextEdit->setPlainText(toMemBlockLabel);

		// loader to executor
		executor = new Executor(memory, nixbpe, registers, loader->GetEndRecord()->executionAddr);

		ui->RegsPCContentLable->setText(QString::fromStdString(registers->Store(PC)));
		ui->RegsAContentLable->setText(QString::fromStdString(registers->Store(A)));
		ui->RegsXContentLable->setText(QString::fromStdString(registers->Store(X)));
		ui->RegsLContentLable->setText(QString::fromStdString(registers->Store(L)));
		ui->RegsBContentLable->setText(QString::fromStdString(registers->Store(B)));
		ui->RegsSContentLable->setText(QString::fromStdString(registers->Store(S)));
		ui->RegsTContentLable->setText(QString::fromStdString(registers->Store(T)));
		ui->RegsSWContentLable->setText(QString::fromStdString(registers->Store(SW)));
	} else {
		MainWindow::popupWindows(QString("未選擇檔案！"));
	}
}

void MainWindow::popupWindows(QString msg) {
	QMessageBox qmsg(QMessageBox::NoIcon, tr("訊息視窗"), QString(), QMessageBox::Ok, 0);

	qmsg.setStyleSheet("QMessageBox {background-color: rgb(235, 235, 235); font-family: 微軟正黑體; font-size: 16px;}");

	if (NULL != qmsg.button(QMessageBox::Ok)) {
		qmsg.button(QMessageBox::Ok)->setText("確定");
		qmsg.button(QMessageBox::Ok)->setStyleSheet("QPushButton {width: 70px; height: 25px;background-color: rgb(139, 139, 139);color: #fff;border: none;border-radius: 5px; align}QPushButton::hover {background-color:rgb(206, 206, 206);color: rgb(45, 39, 39);}QPushButton::pressed {background-color: rgb(165, 165, 165);border-bottom: 2px solid rgb(65, 65, 65);}");
	}

	qmsg.setText(msg);
	qmsg.exec();
}

void MainWindow::on_OneStepBtn_clicked() {
	string CurOpInstructStr = "";

	if (OpenFileState) {
		// temp pc counter, temp format
		int tPC, tFormat;

		if (!(ProgramState = executor->oneStep(&CurOpInstructStr, &tPC))) {
			tFormat = this->executor->GetCurrentFormat();

			// SET TEXT TO GUI
			ui->CurInstructContentLable->setText(QString::fromStdString(CurOpInstructStr));

			ui->RegsPCContentLable->setText(QString::fromStdString(registers->Store(PC)));
			ui->RegsAContentLable->setText(QString::fromStdString(registers->Store(A)));
			ui->RegsXContentLable->setText(QString::fromStdString(registers->Store(X)));
			ui->RegsLContentLable->setText(QString::fromStdString(registers->Store(L)));
			ui->RegsBContentLable->setText(QString::fromStdString(registers->Store(B)));
			ui->RegsSContentLable->setText(QString::fromStdString(registers->Store(S)));
			ui->RegsTContentLable->setText(QString::fromStdString(registers->Store(T)));
			ui->RegsSWContentLable->setText(QString::fromStdString(registers->Store(SW)));

			string stdStr = this->memory->Output();
			QString toMemBlockLabel = QString::fromStdString(stdStr);
			ui->MemeryDisplayPlainTextEdit->setPlainText(toMemBlockLabel);

			tPC = (tPC - this->memory->getLoadAddr()) * 2;
			highlightInst(tPC, tFormat * 2);
		} else {
			popupWindows(QString("程式已執行完畢"));
		}
	} else {
		popupWindows(QString("尚未載入Object Program！"));
	}
}

void MainWindow::on_RemoveObpBtn_clicked() {
	ui->CurInstructContentLable->setText("");
	ui->RegsPCContentLable->setText("000000");
	ui->RegsAContentLable->setText("000000");
	ui->RegsXContentLable->setText("000000");
	ui->RegsLContentLable->setText("FFFFFF");
	ui->RegsBContentLable->setText("000000");
	ui->RegsSContentLable->setText("000000");
	ui->RegsTContentLable->setText("000000");
	ui->RegsSWContentLable->setText("000000");
	ui->LoadAddrLineEdit->setText("000000");
	// enable input loading address
	ui->LoadAddrLineEdit->setDisabled(false);

	ui->MemeryDisplayPlainTextEdit->setPlainText("");
	ui->OpenFileStatusLable->setStyleSheet(QString("background-image: url(:/Image/FileNotSelected.png);"));
	ui->OpenFileTextEdit->setText("");

	// setWindow Title
	this->setWindowTitle(QString("單步執行器"));

	delete loader;
	delete memory;
	delete nixbpe;
	delete registers;

	loader = new Loader();
	memory = new MemoryManager();
	nixbpe = new NIXBPE();
	registers = new Registers();
	OpenFileState = false;
	ProgramState = false;
}

void MainWindow::on_FullExecBtn_clicked() {
	if (OpenFileState) {
		// render new object
		Loader *loaderOut = new Loader();
		MemoryManager *memoryOut = new MemoryManager();
		NIXBPE *nixbpeOut = new NIXBPE();
		Registers *registersOut = new Registers();
		Executor *executorOut;

		// process loadAddrStr from LoadAddrTextEdit insure input in 0-9, A-F, a-f
		QString loadAddrStr = ui->LoadAddrLineEdit->text();

		// get file name from edit text box
		string fileName = ui->OpenFileTextEdit->toPlainText().toLocal8Bit().toStdString();

		if (!loaderOut->LoadObjectProgram(fileName, loadAddrStr.toStdString())) {
			popupWindows(QString("輸出程式執行檔，發生錯誤..."));
			delete loaderOut;
			delete memoryOut;
			delete nixbpeOut;
			delete registersOut;
			return;
		}

		memoryOut->LoadInMemory(loaderOut);

		// loader to executor
		executorOut = new Executor(memoryOut, nixbpeOut, registersOut, loaderOut->GetEndRecord()->executionAddr);

		QString fn = QFileDialog::getSaveFileName(this, tr("儲存檔案為..."), QString(), tr("Text files (*.txt);;All Files (*)"));
		string file2saved, eMsg;
		if (fn.isEmpty()) {
			popupWindows(QString("未輸入欲儲存檔案名稱"));
		} else {
			file2saved = fn.toLocal8Bit().toStdString();
			if (executorOut->SaveFileResult(file2saved, &eMsg)) {
				QMessageBox autoOpenFileAskMag(QMessageBox::NoIcon, "訊息視窗", "儲存完畢，自動開啟檔案？...", QMessageBox::Yes | QMessageBox::No, NULL);
				autoOpenFileAskMag.setStyleSheet("QMessageBox {background-color: rgb(235, 235, 235); font-family: 微軟正黑體; font-size: 16px;}");

				if (NULL != autoOpenFileAskMag.button(QMessageBox::Yes)) {
					autoOpenFileAskMag.button(QMessageBox::Yes)->setText("確定");
					autoOpenFileAskMag.button(QMessageBox::Yes)->setStyleSheet("QPushButton {width: 70px; height: 25px;background-color: rgb(139, 139, 139);color: #fff;border: none;border-radius: 5px; align}QPushButton::hover {background-color:rgb(206, 206, 206);color: rgb(45, 39, 39);}QPushButton::pressed {background-color: rgb(165, 165, 165);border-bottom: 2px solid rgb(65, 65, 65);}");
				}
				if (NULL != autoOpenFileAskMag.button(QMessageBox::No)) {
					autoOpenFileAskMag.button(QMessageBox::No)->setText("取消");
					autoOpenFileAskMag.button(QMessageBox::No)->setStyleSheet("QPushButton {width: 70px; height: 25px;background-color: rgb(139, 139, 139);color: #fff;border: none;border-radius: 5px; align}QPushButton::hover {background-color:rgb(206, 206, 206);color: rgb(45, 39, 39);}QPushButton::pressed {background-color: rgb(165, 165, 165);border-bottom: 2px solid rgb(65, 65, 65);}");
				}

				if(autoOpenFileAskMag.exec() == QMessageBox::Yes){
					string tmp = "file:///" + fn.toStdString();
					QDesktopServices::openUrl (QUrl(QString::fromStdString (tmp), QUrl::TolerantMode));
				}

				delete loaderOut;
				delete memoryOut;
				delete nixbpeOut;
				delete registersOut;
				delete executorOut;
			} else {
				eMsg = "檔案儲存過程中，發生未知錯誤，請通知作者...\n" + eMsg;
				popupWindows(QString::fromStdString(eMsg));
			}
		}
	} else {
		popupWindows(QString("尚未載入Object Program！"));
	}
}

bool MainWindow::validator(QString&input) const {
	string tmpstr = input.toStdString();
	int x = 0;

	for (unsigned int i = 0; i < tmpstr.length(); i++) {
		x = tmpstr.at(i) - '0';
		//0 9 17 22 49 54
		if (!(x >= 0 && x <= 9) && !(x >= 17 && x <= 22) && !(x >= 49 && x <= 54)) {
			return (false);
		}
	}

	return (true);
}

// pos is string index, len highlight number
void MainWindow::highlightInst(int pos, int len) {
	QList < QTextEdit::ExtraSelection > extraSelections;
	QBrush backBrush(QColor("#FFECB3"));
	QPen outlinePen(Qt::gray, 1);
	QTextEdit::ExtraSelection selection;

	// get plain text block row and the pos of the row
	int strLen = (pos + 1) % 50;
	int strPos = pos % 50;
	int row = (int) pos / 50 + 1;
	int rowPos = strPos + 11 + (int) (strLen / 10);

	// use row and the pos of the row to highlight the instruction
	int begin, end, midEnd, midBegin, overNum;

	ui->MemeryDisplayPlainTextEdit->verticalScrollBar()->setValue(row);

	// if across row
	// if not across row
	if (strPos + len > 50) {
		QTextBlock block = ui->MemeryDisplayPlainTextEdit->document()->findBlockByLineNumber(row);
		if (block.isValid()) {
			overNum = rowPos + len - 65;
			begin = rowPos + block.position();
			midEnd = begin + (len - overNum);

			selection.cursor = QTextCursor(ui->MemeryDisplayPlainTextEdit->document());
			selection.cursor.setPosition(begin, QTextCursor::MoveAnchor);
			selection.cursor.setPosition(midEnd, QTextCursor::KeepAnchor);
			selection.format.setBackground(backBrush);
			selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);
			extraSelections.append(selection);
		}

		QTextBlock block2 = ui->MemeryDisplayPlainTextEdit->document()->findBlockByLineNumber(row + 1);
		if (block2.isValid()) {
			midBegin = 11 + block2.position();
			end = midBegin + overNum;

			selection.cursor = QTextCursor(ui->MemeryDisplayPlainTextEdit->document());
			selection.cursor.setPosition(midBegin, QTextCursor::MoveAnchor);
			selection.cursor.setPosition(end, QTextCursor::KeepAnchor);
			selection.format.setBackground(backBrush);
			selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);
			extraSelections.append(selection);
		}
	} else {
		QTextBlock block = ui->MemeryDisplayPlainTextEdit->document()->findBlockByLineNumber(row);
		// if instruction mid have space character
		if ((strPos % 10 + len) > 10) {
//			begin = rowPos = 30
//			midEnd = begin + (len - overnum)
//			midBegin = midEnd + (space)
//			end = midBegin + overnum


			if (block.isValid()) {
				begin = rowPos + block.position();
				overNum = strPos % 10 + len - 10;
				midEnd = begin + (len - overNum);
				midBegin = midEnd + 1;
				end = midBegin + overNum;

				selection.cursor = QTextCursor(ui->MemeryDisplayPlainTextEdit->document());
				selection.cursor.setPosition(begin, QTextCursor::MoveAnchor);
				selection.cursor.setPosition(midEnd, QTextCursor::KeepAnchor);
				selection.format.setBackground(backBrush);
				selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);
				extraSelections.append(selection);

				selection.cursor.setPosition(midBegin, QTextCursor::MoveAnchor);
				selection.cursor.setPosition(end, QTextCursor::KeepAnchor);
				selection.format.setBackground(backBrush);
				selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);
				extraSelections.append(selection);
			}
		} else {
			if (block.isValid()) {
				begin = rowPos + block.position();
				end = begin + len;

				selection.cursor = QTextCursor(ui->MemeryDisplayPlainTextEdit->document());
				selection.cursor.setPosition(begin, QTextCursor::MoveAnchor);
				selection.cursor.setPosition(end, QTextCursor::KeepAnchor);
				selection.format.setBackground(backBrush);
				selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);
				extraSelections.append(selection);
			}
		}
	}
	ui->MemeryDisplayPlainTextEdit->setExtraSelections(extraSelections);
}
