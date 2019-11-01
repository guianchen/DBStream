/*
 * appendingIndex.cpp
 *
 *  Created on: 2019年3月12日
 *      Author: liwei
 */
#include "appendingIndex.h"
namespace DATABASE {

	void appendingIndex::appendUint8Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<uint8_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(uint8_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(uint8_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(uint8_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}

	void appendingIndex::appendInt8Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<int8_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(int8_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(int8_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(int8_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendUint16Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<uint16_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(uint16_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(uint16_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(uint16_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendInt16Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<int16_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(int16_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(int16_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(int16_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendUint32Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<uint32_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(uint32_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(uint32_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(uint32_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendInt32Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<int32_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(int32_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(int32_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(int32_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendUint64Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<uint64_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(uint64_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(uint64_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(uint64_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendInt64Index(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<int64_t> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(int64_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(int64_t*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(int64_t*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendFloatIndex(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<float> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(float*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(float*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(float*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendDoubleIndex(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<double> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key = *(double*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key = *(double*)r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key = *(double*)r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendBinaryIndex(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<binaryType> c;
		if (r->head->minHead.type == DATABASE_INCREASE::R_INSERT || r->head->minHead.type == DATABASE_INCREASE::R_DELETE)
		{
			c.key.data = r->column(index->m_columnIdxs[0]);
			c.key.size = r->varColumnSize(index->m_columnIdxs[0]);
			index->m_varSize += c.key.size + sizeof(uint16_t);
			appendIndex(index, r, &c, id, false);
		}
		else if (r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE)
		{
			c.key.data = r->column(index->m_columnIdxs[0]);
			appendIndex(index, r, &c, id, false);
			if (r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
			{
				c.key.data = r->oldColumnOfUpdateType(index->m_columnIdxs[0]);
				c.key.size = r->oldVarColumnSizeOfUpdateType(index->m_columnIdxs[0], c.key.data);
				appendIndex(index, r, &c, id, true);
			}
		}
		else
			abort();
	}
	void appendingIndex::appendUnionIndex(appendingIndex* index, const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		KeyTemplate<unionKey> c;
		c.key.key = unionKey::initKey(index->m_arena, &index->m_ukMeta, index->m_columnIdxs, index->m_columnCount, r, false);
		if (!index->m_ukMeta.m_fixed)
			index->m_varSize += *(uint16_t*)(c.key.key + index->m_ukMeta.m_size + sizeof(uint16_t));
		c.key.meta = &index->m_ukMeta;
		appendIndex(index, r, &c, id, false);
		if ((r->head->minHead.type == DATABASE_INCREASE::R_UPDATE || r->head->minHead.type == DATABASE_INCREASE::R_REPLACE) && r->isKeyUpdated(index->m_columnIdxs, index->m_columnCount))
		{
			c.key.key = unionKey::initKey(index->m_arena, &index->m_ukMeta, index->m_columnIdxs, index->m_columnCount, r, true);
			appendIndex(index, r, &c, id, true);
		}
	}
	appendingIndex::appendingIndex(uint16_t* columnIdxs, uint16_t columnCount, const META::tableMeta* meta, leveldb::Arena* arena) :
		m_columnIdxs(columnIdxs), m_columnCount(columnCount), m_meta(meta), m_arena(arena), m_localArena(arena == nullptr), m_allCount(0), m_keyCount(0), m_varSize(0)
	{
		if (m_arena == nullptr)
			m_arena = new leveldb::Arena();
		if (columnCount > 0)
		{
			if (!m_ukMeta.init(columnIdxs, columnCount, meta))
			{
				m_type = META::COLUMN_TYPE::T_MAX_TYPE;
				return;
			}
			m_type = META::COLUMN_TYPE::T_UNION;
		}
		else
		{
			m_type = meta->m_columns[columnIdxs[0]].m_columnType;
		}
		switch (m_type)
		{
		case META::COLUMN_TYPE::T_INT8:
			m_comp = new keyComparator<int8_t>;
			m_index = new leveldb::SkipList< KeyTemplate<int8_t>*, keyComparator<int8_t> >(*static_cast<keyComparator<int8_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_UINT8:
			m_comp = new keyComparator<uint8_t>;
			m_index = new leveldb::SkipList< KeyTemplate<uint8_t>*, keyComparator<uint8_t> >(*static_cast<keyComparator<uint8_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_INT16:
			m_comp = new keyComparator<int16_t>;
			m_index = new leveldb::SkipList< KeyTemplate<int16_t>*, keyComparator<int16_t> >(*static_cast<keyComparator<int16_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_UINT16:
			m_comp = new keyComparator<uint16_t>;
			m_index = new leveldb::SkipList< KeyTemplate<uint16_t>*, keyComparator<uint16_t> >(*static_cast<keyComparator<uint16_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_INT32:
			m_comp = new keyComparator<int32_t>;
			m_index = new leveldb::SkipList< KeyTemplate<int32_t>*, keyComparator<int32_t> >(*static_cast<keyComparator<int32_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_UINT32:
			m_comp = new keyComparator<uint32_t>;
			m_index = new leveldb::SkipList< KeyTemplate<uint32_t>*, keyComparator<uint32_t> >(*static_cast<keyComparator<uint32_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_INT64:
			m_comp = new keyComparator<int64_t>;
			m_index = new leveldb::SkipList< KeyTemplate<int64_t>*, keyComparator<int64_t> >(*static_cast<keyComparator<int64_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_TIMESTAMP:
		case META::COLUMN_TYPE::T_UINT64:
			m_comp = new keyComparator<uint64_t>;
			m_index = new leveldb::SkipList< KeyTemplate<uint64_t>*, keyComparator<uint64_t> >(*static_cast<keyComparator<uint64_t>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_FLOAT:
			m_comp = new keyComparator<float>;
			m_index = new leveldb::SkipList< KeyTemplate<float>*, keyComparator<float> >(*static_cast<keyComparator<float>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_DOUBLE:
			m_comp = new keyComparator<double>;
			m_index = new leveldb::SkipList< KeyTemplate<double>*, keyComparator<double> >(*static_cast<keyComparator<double>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_BLOB:
		case META::COLUMN_TYPE::T_STRING:
			m_comp = new keyComparator<binaryType>;
			m_index = new leveldb::SkipList< KeyTemplate<binaryType>*, keyComparator<binaryType> >(*static_cast<keyComparator<binaryType>*>(m_comp), m_arena);
			break;
		case META::COLUMN_TYPE::T_UNION:
			m_comp = new keyComparator<unionKey>;
			m_index = new leveldb::SkipList< KeyTemplate<unionKey>*, keyComparator<unionKey> >(*static_cast<keyComparator<unionKey>*>(m_comp), m_arena);
			break;
		default:
			abort();
		}

	};
	appendingIndex::~appendingIndex()
	{
		if (m_index != nullptr)
		{
			switch (m_type)
			{
			case META::COLUMN_TYPE::T_INT8:
				delete static_cast<leveldb::SkipList< KeyTemplate<int8_t>*, keyComparator<int8_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_UINT8:
				delete static_cast<leveldb::SkipList< KeyTemplate<uint8_t>*, keyComparator<uint8_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_INT16:
				delete static_cast<leveldb::SkipList< KeyTemplate<int16_t>*, keyComparator<int16_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_UINT16:
				delete static_cast<leveldb::SkipList< KeyTemplate<uint16_t>*, keyComparator<uint16_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_INT32:
				delete static_cast<leveldb::SkipList< KeyTemplate<int32_t>*, keyComparator<int32_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_UINT32:
				delete static_cast<leveldb::SkipList< KeyTemplate<uint32_t>*, keyComparator<uint32_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_INT64:
				delete static_cast<leveldb::SkipList< KeyTemplate<int64_t>*, keyComparator<int64_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_TIMESTAMP:
			case META::COLUMN_TYPE::T_UINT64:
				delete static_cast<leveldb::SkipList< KeyTemplate<uint64_t>*, keyComparator<uint64_t> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_FLOAT:
				delete static_cast<leveldb::SkipList< KeyTemplate<float>*, keyComparator<float> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_DOUBLE:
				delete static_cast<leveldb::SkipList< KeyTemplate<double>*, keyComparator<double> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_STRING:
			case META::COLUMN_TYPE::T_BLOB:
				delete static_cast<leveldb::SkipList< KeyTemplate<binaryType>*, keyComparator<binaryType> >*>(m_index);
				break;
			case META::COLUMN_TYPE::T_UNION:
				delete static_cast<leveldb::SkipList< KeyTemplate<unionKey>*, keyComparator<unionKey> >*>(m_index);
				break;
			default:
				abort();
			}
		}
		if (m_localArena && m_arena != nullptr)
			delete m_arena;

	}
	typename appendingIndex::appendIndexFunc appendingIndex::m_appendIndexFuncs[] = {
		appendUnionIndex,
		appendUint8Index,appendInt8Index,appendUint16Index,appendInt16Index,
		appendUint32Index,appendInt32Index,appendUint64Index,appendInt64Index,nullptr,
		appendFloatIndex,appendDoubleIndex,nullptr,appendUint64Index,nullptr,nullptr,nullptr,nullptr,
		appendBinaryIndex ,appendBinaryIndex,nullptr,nullptr,nullptr,nullptr };
	void  appendingIndex::append(const DATABASE_INCREASE::DMLRecord* r, uint32_t id)
	{
		m_appendIndexFuncs[static_cast<int>(m_type)](this, r, id);
		m_allCount++;
	}
	template<>
	void appendingIndex::createFixedSolidIndex<unionKey>(char* data, appendingIndex::iterator<unionKey>& iter, uint16_t keySize)
	{
		char* indexPos = data + sizeof(uint32_t), * externCurretPos = data + keySize * m_keyCount;
		*(uint32_t*)data = m_keyCount;
		do
		{
			const keyChildInfo* k = iter.keyDetail();
			memcpy(indexPos, iter.key().key, keySize);
			if (k->count == 1)
			{
				*(uint32_t*)(indexPos + keySize) = k->subArray[0];
			}
			else
			{
				if (k->count < 0x7f)
				{
					*(uint32_t*)(indexPos + keySize) = (k->count << 24) | (externCurretPos - data);
				}
				else
				{
					*(uint32_t*)(indexPos + keySize) = (0x80u << 24) | (externCurretPos - data);
					*(uint32_t*)externCurretPos = k->count;
					externCurretPos += sizeof(uint32_t);
				}
				memcpy(externCurretPos, k->subArray, sizeof(uint32_t) * k->count);
			}
			indexPos += keySize + sizeof(uint32_t);
		} while (iter.nextKey());
	}
	template<>
	void appendingIndex::createVarSolidIndex<unionKey>(char* data, appendingIndex::iterator<unionKey>& iter)
	{
		char* indexPos = data + sizeof(uint32_t), * externCurretPos = data + sizeof(uint32_t) * (m_keyCount + 1);
		*(uint32_t*)data = m_keyCount;
		int kc = 0;
		do
		{
			kc++;
			const keyChildInfo* k = iter.keyDetail();
			*(uint32_t*)indexPos = externCurretPos - data;
			indexPos += sizeof(uint32_t);
			*(uint16_t*)externCurretPos = m_ukMeta.m_size + *(const uint16_t*)(iter.key().key + m_ukMeta.m_size);
			memcpy(externCurretPos + sizeof(uint16_t), &iter.key(), *(uint16_t*)externCurretPos);
			externCurretPos += sizeof(uint16_t) + *(uint16_t*)externCurretPos;
			*(uint32_t*)externCurretPos = k->count;
			memcpy(externCurretPos + sizeof(uint32_t), k->subArray, sizeof(uint32_t) * k->count);
			externCurretPos += sizeof(uint32_t) + sizeof(uint32_t) * k->count;
		} while (iter.nextKey());
		*(uint32_t*)indexPos = externCurretPos - data;
	}
	template<>
	void appendingIndex::createVarSolidIndex<binaryType>(char* data, appendingIndex::iterator<binaryType>& iter)
	{
		char* indexPos = data + sizeof(uint32_t), * externCurretPos = data + sizeof(uint32_t) * (m_keyCount + 1);
		*(uint32_t*)data = m_keyCount;
		do
		{
			const keyChildInfo* k = iter.keyDetail();
			*(uint32_t*)indexPos = externCurretPos - data;
			indexPos += sizeof(uint32_t);
			*(uint16_t*)externCurretPos = iter.key().size;
			memcpy(externCurretPos + sizeof(uint16_t), iter.key().data, *(uint16_t*)externCurretPos);
			externCurretPos += iter.key().size;
			*(uint32_t*)externCurretPos = k->count;
			memcpy(externCurretPos + sizeof(uint32_t), k->subArray, sizeof(uint32_t) * k->count);
			externCurretPos += sizeof(uint32_t) + sizeof(uint32_t) * k->count;
		} while (iter.nextKey());
		*(uint32_t*)indexPos = externCurretPos - data;
	}


}
