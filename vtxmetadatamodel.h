/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef VTXMETADATAMODEL_H
#define VTXMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include "vtxhelper.h"

class VTXMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(VTXMetaDataModel)
public:
    VTXMetaDataModel(const QString &path);
    virtual ~VTXMetaDataModel();

    virtual QList<TagModel* > tags() const override;

private:
    QList<TagModel* > m_tags;
    VTXHelper* m_vtx;
};

class VTXFileTagModel : public TagModel
{
public:
    VTXFileTagModel(VTXHelper* vtx);
    virtual ~VTXFileTagModel();

    virtual QString name() const override;
    virtual QList<Qmmp::MetaData> keys() const override;
    virtual QString value(Qmmp::MetaData key) const override;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override;

private:
    VTXHelper* m_vtx;
};

#endif // VTXMETADATAMODEL_H
