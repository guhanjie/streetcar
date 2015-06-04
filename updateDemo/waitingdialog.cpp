#include "waitingdialog.h"

#include <QHBoxLayout>

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent)
{
    m_ProgressBar = new QProgressBar(this);
    m_CurrentValue = m_MaxValue = m_UpdateInterval = 0;
    m_ProgressBar->setRange(0, 100);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(UpdateSlot()));
    m_ProgressBar->setTextVisible(false);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_ProgressBar);
    setLayout(layout);
}

void WaitingDialog::UpdateSlot()
{
    m_CurrentValue++;
    if( m_CurrentValue == m_MaxValue )
        m_CurrentValue = 0;
    m_ProgressBar->setValue(m_CurrentValue);
}

void WaitingDialog::Start(int interval/* =100 */, int maxValue/* =100 */)
{
    m_UpdateInterval = interval;
    m_MaxValue = maxValue;
    m_Timer.start(m_UpdateInterval);
    m_ProgressBar->setRange(0, m_MaxValue);
    m_ProgressBar->setValue(0);
}

void WaitingDialog::Stop()
{
    m_Timer.stop();
}
