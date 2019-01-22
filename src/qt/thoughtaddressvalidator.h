// Copyright (c) 2011-2014 The Thought Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef THOUGHT_QT_THOUGHTADDRESSVALIDATOR_H
#define THOUGHT_QT_THOUGHTADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ThoughtAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ThoughtAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Thought address widget validator, checks for a valid thought address.
 */
class ThoughtAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ThoughtAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // THOUGHT_QT_THOUGHTADDRESSVALIDATOR_H
