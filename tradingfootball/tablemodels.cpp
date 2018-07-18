#include "ui_trading.h"
#include "tablemodels.h"

bool PlayerListViewFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    if (!myIsEnabled) return true;
    PlayerListModal * model = dynamic_cast<PlayerListModal *>(sourceModel());
    if (model==NULL) return true;
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    ViewModel * item = model->getItemByIndex(index);
    if (item == NULL) return true;

    GamesFilter gameFilter = myuit->openrb->isChecked() ? GamesFilter::OpenGames : GamesFilter::LockedGames;
    QString gameId = item->propertyValue<PropertyNames::Game_ID>().toString();
    if ( gameId == "" && gameFilter == GamesFilter::LockedGames)
        return false;

    if (gameId != "" && myGameModel!=NULL){

        if (gameFilter != GamesFilter::All) {
            QVariant vvalue;
            GameStatus_Status gameStatus;
            bool propertyFound = myGameModel->itemPropertyValue<PropertyNames::Game_Status>(gameId,vvalue);
            if (!propertyFound) return false;
            gameStatus = qvariant_cast<GameStatus_Status>(vvalue);
            if (!GameViewFilterProxyModel::testGameStatus(gameFilter,gameStatus))
                return false;
        }
    }

    if ( myuit->groupBoxPlayerFilterGame->isChecked() ) {
        if ( myuit->teamComboFilter->isEnabled() ) {
            QString team = myuit->teamComboFilter->currentText().trimmed().toUpper();
            if (team != "ALL"){
                QString teamId = item->propertyValue<PropertyNames::Team_ID>().toString();
                if (teamId != team)
                    return false;
            }
        }

        if ( myuit->posComboFilter->isEnabled() ) {
            QString position= myuit->posComboFilter->currentText().trimmed().toUpper();
            if (position != "ALL"){
                QString playerPosition=item->propertyValue<PropertyNames::Position>().toString();
                if (playerPosition != position)
                    return false;
            }
        }
    }

    if ( myuit->groupBoxPlayerFilterTrading->isChecked() ) {
        if ( myuit->mypositionsFilter->isChecked() ) {
            if ( item->propertyValue<PropertyNames::MYPOS>().toInt() == 0) {
                if ( item->propertyValue<PropertyNames::MYAVG>().toInt() == 0 )
                    return false;
            }
        }

       // if ( myuit->mypositionsFilter->isChecked() ) {
       //     return false;
       // }

    }


    if ( myuit->groupBoxPlayerFilterMarket->isChecked() ) {
        if ( myuit->activefilter->isChecked() ) {
            if ( item->propertyValue<PropertyNames::VOLUME>().toInt() == 0)
                return false;
        }

        if ( myuit->liquidfilter->isChecked() ) {
            if ( item->propertyValue<PropertyNames::BID>().toInt() == 0 )
                return false;
            if ( item->propertyValue<PropertyNames::ASK>().toInt() == 0 )
                return false;
        }


        if ( myuit->moversfiler->isChecked() ) {
            int last = item->propertyValue<PropertyNames::LAST>().toInt();
            int change = abs(item->propertyValue<PropertyNames::CHANGE>().toInt());
            if ( change == 0 ) return false;

            if ( last > 0 ) {
                if ( (change / last ) < .20 ) return false;
            }
        }

    }


    return true;
}

