#include "stdafx.h"
#include "KernelView.h"
#include "../../TaskExplorer.h"
#include "PoolView.h"
#include "DriversView.h"
#include "NtObjectView.h"
#include "AtomView.h"


CKernelView::CKernelView(QWidget *parent)
	:QWidget(parent)
{
	m_pMainLayout = new QGridLayout();
	//m_pMainLayout->setMargin(3);
	this->setLayout(m_pMainLayout);

	m_pTabs = new QTabWidget();
	//m_pTabs->setDocumentMode(true);
	m_pTabs->setTabPosition(QTabWidget::South);
	//m_pTabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	m_pMainLayout->addWidget(m_pTabs, 0, 0);
	
	m_pPoolView = new CPoolView(this);
	m_pTabs->addTab(m_pPoolView, tr("Pool table"));

	m_pDriversView = new CDriversView(this);
	m_pTabs->addTab(m_pDriversView, tr("Drivers"));

	m_pNtObjectView = new CNtObjectView(this);
	m_pTabs->addTab(m_pNtObjectView, tr("Nt Objects"));

	m_pAtomView = new CAtomView(this);
	m_pTabs->addTab(m_pAtomView, tr("Atom table"));



	m_pInfoWidget = new QWidget();
	m_pInfoLayout = new QHBoxLayout();
	m_pInfoLayout->setMargin(0);
	m_pInfoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	m_pInfoWidget->setLayout(m_pInfoLayout);
	m_pMainLayout->addWidget(m_pInfoWidget, 1, 0);

	m_pInfoWidget1 = new QWidget();
	m_pInfoLayout1 = new QVBoxLayout();
	m_pInfoLayout1->setMargin(0);
	m_pInfoWidget1->setLayout(m_pInfoLayout1);
	m_pInfoLayout->addWidget(m_pInfoWidget1);

	m_pInfoWidget2 = new QWidget();
	m_pInfoLayout2 = new QVBoxLayout();
	m_pInfoLayout2->setMargin(0);
	m_pInfoWidget2->setLayout(m_pInfoLayout2);
	m_pInfoLayout->addWidget(m_pInfoWidget2);

	m_pInfoLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

	//////////////////////////////////////////////////////////

	m_pPPoolBox = new QGroupBox(tr("Paged pool"));
	m_pPPoolBox->setMinimumWidth(150);
	m_pPPoolLayout = new QGridLayout();
	m_pPPoolBox->setLayout(m_pPPoolLayout);
	m_pInfoLayout1->addWidget(m_pPPoolBox, 0, 0);
	m_pInfoLayout1->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	int row = 0;
	m_pPPoolLayout->addWidget(new QLabel(tr("Working set")), row, 0);
	m_pPPoolWS = new QLabel();
	m_pPPoolLayout->addWidget(m_pPPoolWS, row++, 1);

	m_pPPoolLayout->addWidget(new QLabel(tr("Virtual size")), row, 0);
	m_pPPoolVSize = new QLabel();
	m_pPPoolLayout->addWidget(m_pPPoolVSize, row++, 1);

	//m_pPPoolLayout->addWidget(new QLabel(tr("Paged limit")), row, 0);
	//m_pPPoolLimit = new QLabel();
	//m_pPPoolLayout->addWidget(m_pPPoolLimit, row++, 1);

	////////////////////////////////////////

	m_pNPPoolBox = new QGroupBox(tr("Non-paged pool"));
	m_pNPPoolBox->setMinimumWidth(150);
	m_pNPPoolLayout = new QGridLayout();
	m_pNPPoolBox->setLayout(m_pNPPoolLayout);
	m_pInfoLayout2->addWidget(m_pNPPoolBox);
	m_pInfoLayout2->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	row = 0;
	m_pNPPoolLayout->addWidget(new QLabel(tr("Usage")), row, 0);
	m_pNPPoolUsage = new QLabel();
	m_pNPPoolLayout->addWidget(m_pNPPoolUsage, row++, 1);

	m_pNPPoolLayout->addWidget(new QLabel(), row++, 0);

	//m_pNPPoolLayout->addWidget(new QLabel(tr("Limit")), row, 0);
	//m_pNPPoolLimit = new QLabel();
	//m_pNPPoolLayout->addWidget(m_pNPPoolLimit, row++, 1);

	//m_pPoolLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 3);

	// Note: Winsows 10 starting with 1809 does not longer provide a value for:
	//			PagedPoolAllocs, PagedPoolFrees, NonPagedPoolAllocs, NonPagedPoolFrees

	/*setObjectName(parent->objectName());
	QByteArray Columns = theConf->GetBlob(objectName() + "/KernelView_Columns");
	if (!Columns.isEmpty())
		m_pSwapList->GetView()->header()->restoreState(Columns);*/

	m_pTabs->setCurrentIndex(theConf->GetValue(objectName() + "/KernelView_Tab").toInt());
}


CKernelView::~CKernelView()
{
	//theConf->SetBlob(objectName() + "/KernelView_Columns", m_pSwapList->GetView()->header()->saveState());
	theConf->SetValue(objectName() + "/KernelView_Tab", m_pTabs->currentIndex());
}

void CKernelView::Refresh()
{
	if(m_pTabs->currentWidget() == m_pPoolView)
		m_pPoolView->Refresh();
	else if(m_pTabs->currentWidget() == m_pDriversView)
		m_pDriversView->Refresh();
	else if(m_pTabs->currentWidget() == m_pNtObjectView)
		m_pNtObjectView->Refresh();
	else if(m_pTabs->currentWidget() == m_pAtomView)
		m_pAtomView->Refresh();

	m_pPPoolWS->setText(FormatSize(theAPI->GetPersistentPagedPool()));
	m_pPPoolVSize->setText(FormatSize(theAPI->GetPagedPool()));

	m_pNPPoolUsage->setText(FormatSize(theAPI->GetNonPagedPool()));

	// todo: add limti values make them work on win10
	//m_pPPoolLimit;
	//m_pNPPoolLimit;
}
