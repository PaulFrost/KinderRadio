#include "tag.h"

Tag::Tag(const QString &tagId, QObject *parent) : QObject(parent),
	m_id(tagId)
{

}

Tag::~Tag()
{

}

QString Tag::id() const
{
	return m_id;
}

void Tag::setId(const QString &id)
{
	m_id = id;
}

TagSettings::TagType Tag::type() const
{
	return m_type;
}

void Tag::setType(const TagSettings::TagType &type)
{
	m_type = type;
}
