//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_RECOVERY_WIZARD_H
#define OMNISERVER_RECOVERY_WIZARD_H

#include <QWizard>

class RecoveryWizard : public QWizard {
    Q_OBJECT

public:
    explicit RecoveryWizard(QWidget *parent = nullptr);
};


#endif //OMNISERVER_RECOVERY_WIZARD_H
