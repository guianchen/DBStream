/*
 * appendingBlock.h
 *
 *  Created on: 2019年1月7日
 *      Author: liwei
 */
#include <atomic>
#include <time.h>
#define OS_WIN  //todo
#include "../util/file.h"
#include "../util/skiplist.h"
#include "../meta/metaDataCollection.h"
#include "../util/unorderMapUtil.h"
#include "../message/record.h"
#include "filter.h"
#include "iterator.h"
#include "schedule.h"
#include "block.h"
#include "blockManager.h"
#include <string.h>
#include <glog/logging.h>
#include "../meta/metaData.h"
#include "appendingIndex.h"
#include "../util/arrayList.h"
#include "../util/barrier.h"
#include "../lz4/lz4.h"
namespace STORE
{
class appendingBlockIterator;
class appendingBlockLineIterator;
static constexpr int maxRecordInBlock = 1024 * 128;
class appendingBlock: public block
{
public:
    enum appendingBlockStaus
    {
        B_OK, B_FULL, B_ILLEGAL, B_FAULT
    };
private:
	struct tableData
	{
		uint64_t blockID;
		META::tableMeta * meta;
		appendingIndex * primaryKey;
		appendingIndex ** uniqueKeys;
		arrayList<uint32_t> recordIds;
		arrayList<page*> pages;
		page * current;
		uint32_t pageSize;
		tableData(uint64_t blockID,META::tableMeta * meta , leveldb::Arena *arena , uint32_t _pageSize = 512 * 1024):blockID(blockID),meta(meta),primaryKey(nullptr), uniqueKeys(nullptr),recordIds(arena), pages(arena),current(nullptr), pageSize(_pageSize)
		{
			if (meta != nullptr)
			{
				if (meta->m_primaryKey.count > 0)
					primaryKey = new appendingIndex(meta->m_primaryKey.keyIndexs, meta->m_primaryKey.count,meta, arena);
				if (meta->m_uniqueKeysCount > 0)
				{
					uniqueKeys = new appendingIndex*[meta->m_uniqueKeysCount];
					for(uint16_t i =0;i< meta->m_uniqueKeysCount;i++)
						uniqueKeys[i] = new appendingIndex(meta->m_uniqueKeys[i].keyIndexs, meta->m_uniqueKeys[i].count, meta, arena);
				}
			}
		}
		void init(uint64_t blockID, META::tableMeta * meta, leveldb::Arena *arena, uint32_t _pageSize = 512 * 1024)
		{
			blockID = blockID;
			this->meta = meta;
			if (meta != nullptr)
			{
				if (meta->m_primaryKey.count > 0)
					primaryKey = new appendingIndex(meta->m_primaryKey.keyIndexs, meta->m_primaryKey.count, meta, arena);
				else
					primaryKey = nullptr;
				if (meta->m_uniqueKeysCount > 0)
				{
					uniqueKeys = new appendingIndex*[meta->m_uniqueKeysCount];
					for (uint16_t i = 0; i < meta->m_uniqueKeysCount; i++)
						uniqueKeys[i] = new appendingIndex(meta->m_uniqueKeys[i].keyIndexs, meta->m_uniqueKeys[i].count, meta, arena);
				}
				else
					uniqueKeys = nullptr;
			}
			else
			{
				primaryKey = nullptr;
				uniqueKeys = nullptr;
			}
			recordIds.init(arena);
			pages.init(arena);
			current = nullptr;
			pageSize = _pageSize;//todo
		}
	};

    std::atomic<uint32_t> m_ref;
    uint32_t * m_recordIDs;
	size_t m_size;
	size_t m_maxSize;
	std::atomic<uint32_t> m_committedRecordID;
    std::atomic<bool> m_ended;
    std::string m_path;
    appendingBlockStaus m_status;
	leveldb::Arena m_arena;
	tableData m_defaultData;
	std::map<uint64_t,tableData*> m_tableDatas;
	blockManager * m_blockManager;
	page ** m_pages;
	uint16_t m_pageNum;
	uint16_t m_maxPageNum;
	tableData m_defaultTableData;//for ddl,heartbeat
    fileHandle m_fd;
    fileHandle m_redoFd;

    int32_t m_redoUnflushDataSize;
    int32_t m_redoFlushDataSize;
    int32_t m_redoFlushPeriod; //micro second
	uint64_t m_txnId;
    clock_t m_lastFLushTime;
	friend class appendingBlockIterator;
	friend class appendingBlockLineIterator;
public:
    appendingBlock(uint32_t flag, const char * logDir, const char * logPrefix,
            uint32_t bufSize,int32_t redoFlushDataSize,int32_t redoFlushPeriod,uint64_t startID, blockManager * manager) :
		m_size(0),m_maxSize(bufSize), m_status(B_OK), m_defaultData(m_blockID,nullptr,&m_arena), m_blockManager(manager), m_pageNum(0), m_defaultTableData(m_blockID,nullptr,&m_arena,4096),
		m_redoFd(0),m_redoUnflushDataSize(0),m_redoFlushDataSize(redoFlushDataSize),
            m_redoFlushPeriod(redoFlushPeriod), m_txnId(0), m_lastFLushTime(0)
    {
        char fileName[256];
        snprintf(fileName,255, "%s/%s.%lu", logDir, logPrefix, m_blockID);
        m_path.assign(fileName);
        m_recordIDs = (uint32_t*)m_blockManager->allocMem(sizeof(uint32_t)*maxRecordInBlock);
		m_maxPageNum = m_maxSize / (32 * 1204);
		m_pages = (page**)m_blockManager->allocMem(sizeof(page*)*m_maxPageNum);
		m_minTime = m_maxTime = 0;
		m_minLogOffset = m_maxLogOffset = 0;
		m_minRecordId = startID;
		m_recordCount = 0;
		m_tableCount = 0;
    }
    int openRedoFile()
    {
        if (0
                > (m_redoFd = openFile((m_path + ".redo").c_str(),true,true,true)))
        {
            LOG(ERROR)<<"open redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
            return -1;
        }
        int fileSize;
        if((fileSize = seekFile(m_redoFd,0,SEEK_END))<m_size)
        {
            LOG(ERROR)<<"eopen redo file :"<<m_path<<".redo failed for file size check failed,expect file size:"
            << m_size <<",actually is "<<fileSize;
            return -1;
        }
        if(fileSize!= m_size)
        {
            if(0!=truncateFile(m_redoFd, m_size))
            {
                LOG(ERROR)<<"ftruncate redo file :"<<m_path<<".redo to size:"<< m_size <<"failed for ferrno:"<<errno<<",error info:"<<strerror(errno);
                return -1;
            }
            if(seekFile(m_redoFd, m_size,SEEK_SET)!= m_size)
            {
                LOG(ERROR)<<"open redo file :"<<m_path<<".redo failed for file size check failed,expect file size:"
                << m_size <<",actually is "<<fileSize;
                return -1;
            }
        }
        return 0;
    }
	inline bool isLegalRecordId(uint64_t recordId)
	{
		return (recordId >= m_minRecordId && recordId < m_minRecordId + m_recordCount);
	}
	inline const char * getRecord(uint64_t recordId)
	{
		uint32_t offset = m_recordIDs[recordId - m_minRecordId];
		return m_pages[pageId(offset)]->pageData + offsetInPage(offset);
	}
	inline uint64_t findRecordIDByOffset(uint64_t offset)
	{
		DATABASE_INCREASE::recordHead* head;
		uint32_t endID = m_recordCount-1;
		if(m_recordCount ==0|| ((const DATABASE_INCREASE::recordHead*)getRecord(m_minRecordId))->logOffset<offset|| ((const DATABASE_INCREASE::recordHead*)getRecord(m_minRecordId+m_recordCount-1))->logOffset>offset)
			return 0;
		int s = 0, e = endID ,m;
		uint64_t midOffset;
		while (s <= e)
		{
			m = (s + e) >> 1;
			midOffset = ((const DATABASE_INCREASE::recordHead*)getRecord(m_minRecordId + m))->logOffset;
			if (midOffset > offset)
				e = m - 1;
			else if (midOffset < offset)
				s = m + 1;
			else
				return m_minRecordId + m;
		}
		if (s >= 0 && s<endID - m_minRecordId && ((const DATABASE_INCREASE::recordHead*)getRecord(m_minRecordId + s))->logOffset > offset)
			return m_minRecordId + s;
		else
			return 0;
	}

    int recoveryFromRedo(META::metaDataCollection *mc)
    {
        if (m_redoFd >0)
			closeFile(m_redoFd);
        if(0>(m_redoFd = openFile((m_path+".redo").c_str(),true,true,false)))
        {
            LOG(ERROR)<<"open redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
            return -1;
        }
        int size = seekFile(m_redoFd,0,SEEK_END);//get fileSize
        if(size<0)
        {
            LOG(ERROR)<<"get size of  redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
            return -1;
        }
        if(size == 0) //empty file
            return 0;
        char * buf = (char*)malloc(size);
        if(buf == NULL)
        {
            LOG(ERROR)<<"alloc "<<size<<" byte memory failed";
            return -1;
        }
        if(0!= seekFile(m_redoFd,0,SEEK_SET))//seek to begin of file
        {
            free(buf);
            LOG(ERROR)<<"leeek to begin of file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
            return -1;
        }
        if(size!=readFile(m_redoFd,buf,size))//read all data one time
        {
            free(buf);
            LOG(ERROR)<<"read redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
            return -1;
        }
        m_flag &= ~BLOCK_FLAG_HAS_REDO;//unset BLOCK_FLAG_HAS_REDO,so call [append] will not write redo file
        DATABASE_INCREASE::recordHead* head = (DATABASE_INCREASE::recordHead*) buf;
        while((char*)head<=buf+size)
        {
            if(((char*)head)+sizeof(DATABASE_INCREASE::recordHead)>buf+size||((char*)head)+head->size>buf+size)//unfinished write ,truncate
            {
                LOG(WARNING)<<"get an incomplete redo data in file:"<<m_path<<".redo ,offset is "<<((char*)head)-buf;
                closeFile(m_redoFd);
                if(0!=openRedoFile())//openRedoFile will truncate file
                {
                    LOG(WARNING)<<"reopen redo file:"<<m_path<<".redo failed";
                    free(buf);
                    m_flag |= BLOCK_FLAG_HAS_REDO; //reset BLOCK_FLAG_HAS_REDO
                    return -1;
                }
                break;
            }
            if(append(DATABASE_INCREASE::createRecord((const char*)head,mc))!=B_OK)
            {
                LOG(ERROR)<<"recoveryFromRedo from  file :"<<m_path<<".redo failed for append data failed";
                free(buf);
                m_flag |= BLOCK_FLAG_HAS_REDO; //reset BLOCK_FLAG_HAS_REDO
                return -1;
            }
            head = (DATABASE_INCREASE::recordHead*)(((char*)head)+ head->size);
        }
        m_flag |= BLOCK_FLAG_HAS_REDO; //reset BLOCK_FLAG_HAS_REDO
        LOG(INFO)<<"recoveryFromRedo from  file :"<<m_path<<".redo success";
        free(buf);
        return 0;
    }
    appendingBlockStaus writeRedo(const char * data)
    {
        if (m_redoFd < 0&&0!=openRedoFile())
            return B_FAULT;
        DATABASE_INCREASE::recordHead* head = (DATABASE_INCREASE::recordHead*) data;
        int writeSize;
        if(head->size!=(writeSize=writeFile(m_redoFd,data,head->size)))
        {
            if(errno==EBADF) //maybe out time or other cause,reopen it
            {
                LOG(WARNING)<<"write redo file:"<<m_path<<".redo failed for "<<strerror(errno)<<" reopen it";
                if (0!=openRedoFile())
                    return B_FAULT;
                return writeRedo(data);
            }
            else
            {
                LOG(ERROR)<<"write redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
                return B_FAULT;
            }
        }
		clock_t now;
        if(m_redoFlushDataSize==0||//m_redoFlushDataSize == 0 means flush immediately
                m_redoFlushPeriod==0|| //m_redoFlushPeriod == 0 also means flush immediately
                (m_redoFlushDataSize>0&&(m_redoUnflushDataSize+=head->size)>=m_redoFlushDataSize)||//check if unflushed data big enough
                (m_redoFlushPeriod>0&& //check if time from last flush is long enough
                        (now = clock(),
							now<m_lastFLushTime||now - m_lastFLushTime > m_redoFlushPeriod*CLOCKS_PER_SEC/1000)))
        {
            if(0!=fsync(m_redoFd))
            {
                LOG(ERROR)<<"fsync redo file :"<<m_path<<".redo failed for errno:"<<errno<<",error info:"<<strerror(errno);
                return B_FAULT;
            }
            m_redoUnflushDataSize = 0;
            m_lastFLushTime = clock();
            return B_OK;
        }
        return B_OK;
    }
	inline tableData *getTableData(META::tableMeta * meta)
	{
		if (likely(meta != nullptr))
		{
			if (meta->userData == nullptr || m_blockID != static_cast<tableData*>(meta->userData)->blockID)
			{
				tableData * table = new tableData(m_blockID, meta, &m_arena);
				meta->userData = table;
				m_tableDatas.insert(std::pair<uint64_t, tableData*>(meta->m_id, table));
				m_tableCount++;
				return table;
			}
			else
				return static_cast<tableData*>(meta->userData);
		}
		else
			return &m_defaultData;
	}
	inline appendingBlockStaus allocMemForRecord(tableData * t, size_t size, void *&mem)
	{
		if (m_recordCount >= maxRecordInBlock)
			return B_FULL;
		if (unlikely(t->current == nullptr || t->current->pageUsedSize + size > t->current->pageSize))
		{
			size_t psize = size > t->pageSize ? size : t->pageSize;
			if (m_pageNum + 1 >= m_maxPageNum || m_size + psize >= m_maxSize)
				return B_FULL;
			t->current = m_blockManager->allocPage(psize);
			t->current->pageId = m_pageNum;
			m_pages[m_pageNum++] = t->current;
			t->pages.append(t->current);
		}
		mem = t->current->pageData + t->current->pageUsedSize;
		return B_OK;
	}
	inline appendingBlockStaus allocMemForRecord(META::tableMeta * table, size_t size,void *&mem)
	{
		return allocMemForRecord(getTableData(table),size,mem);
	}
	inline appendingBlockStaus copyRecord(const DATABASE_INCREASE::record * record)
	{
		tableData *t = getTableData(likely(record->head->type <= DATABASE_INCREASE::R_REPLACE) ? ((DATABASE_INCREASE::DMLRecord*)record)->meta : nullptr);
		page * current = t->current;
		if (unlikely(current == nullptr||current->pageData + current->pageUsedSize != record->data))
		{
			appendingBlockStaus s;
			void *mem;
			if (B_OK != (s = allocMemForRecord(t, record->head->size, mem)))
				return s;
			memcpy(mem, record->data, record->head->size);
			current = t->current;
		}
		setRecordPosition(m_recordIDs[m_recordCount], current->pageId, current->pageUsedSize);
		current->pageUsedSize += record->head->size;
		return B_OK;
	}
    appendingBlockStaus append(const DATABASE_INCREASE::record * record)
    {
		if (m_maxLogOffset > record->head->logOffset)
		{
			LOG(ERROR) << "can not append record to block for record log offset "<<record->head->logOffset<<"is less than max log offset:"<< m_maxLogOffset
				<< "record type:"<<record->head->type;
			return B_ILLEGAL;
		}
		appendingBlockStaus s;
        if ((s = copyRecord(record))!=B_OK)
			return s;
		if (m_flag&BLOCK_FLAG_HAS_REDO)
		{
			appendingBlockStaus rtv;
			if (B_OK != (rtv = writeRedo(record->data)))
			{
				LOG(ERROR) << "write redo log failed";
				return rtv;
			}
		}
		if (record->head->type <= DATABASE_INCREASE::R_REPLACE) //build index
		{
			META::tableMeta * meta = ((const DATABASE_INCREASE::DMLRecord*)record)->meta;
			tableData * table =  static_cast<tableData*>(meta->userData);
			table->recordIds.append(m_recordCount);
			if (table->primaryKey)
				table->primaryKey->append((const DATABASE_INCREASE::DMLRecord*)record, m_recordCount);
			for (int i = 0; i < meta->m_uniqueKeysCount; i++)
				table->uniqueKeys[i]->append((const DATABASE_INCREASE::DMLRecord*)record, m_recordCount);
		}
		barrier;
		m_maxLogOffset = record->head->logOffset;
		if(m_minLogOffset ==0)
			m_minLogOffset = record->head->logOffset;
		if (m_minTime == 0)
			m_minTime = record->head->timestamp;
		if (m_maxTime < record->head->timestamp)
			m_maxTime = record->head->timestamp;
		m_recordCount++;
		if (record->head->txnId > m_txnId || record->head->type == DATABASE_INCREASE::R_DDL)
			m_committedRecordID.store(m_recordCount, std::memory_order_release);
		m_txnId = record->head->txnId;
		return B_OK;
	}

	#define writePage(_page,_size) do{if (_size != writeFile(m_fd, _page, _size))\
		{\
			if(_page!=page)\
				free(page);\
			LOG(ERROR) << "write page to block file " << m_path.append(".bak") << " failed for errno:" << errno << " ," << strerror(errno);\
			rtv = -1;\
			goto END;\
		}\
		tableInfo[tableCount].pageCount++;\
		pageCount++;\
		offset += _size;\
		pageOffsets[pageCount] = offset;\
		pageSize = 0;}while(0);
	int writeRecordToFile(tableDataInfo * tableInfo, recordGeneralInfo * recordInfo,uint64_t &offset)
	{
		int rtv = 0;
		char *page = (char*)malloc(blockPageSize);
		uint32_t pageSize = 0;
		uint16_t pageCount = 0;
		uint32_t tableCount = 0;
		LZ4_stream_t* lz4Stream = LZ4_createStream();
		arrayList<uint32_t>::iterator aiter;
		uint64_t * pageOffsets = (uint64_t*)malloc(2 * sizeof(uint64_t)*m_offset.load(std::memory_order_relaxed) / blockPageSize);
		pageOffsets[0] = offset;
		for (std::map<uint64_t, tableData*>::const_iterator iter = m_tableDatas.begin(); iter != m_tableDatas.end(); iter++)
		{
			LZ4_resetStream(lz4Stream);
			pageSize = 0;
			iter->second->recordIds.begin(aiter);
			tableInfo[tableCount].tableId = iter->first;
			tableInfo[tableCount].startPageId = pageCount;
			do {
				uint32_t recordIdx = aiter.value();
				const char * r = getRecord(recordIdx + m_minRecordId);
				const DATABASE_INCREASE::recordHead * head = (const DATABASE_INCREASE::recordHead*)r;
				recordInfo[recordIdx].logOffset = head->logOffset;
				recordInfo[recordIdx].recordType = head->type;
				recordInfo[recordIdx].timestamp = head->timestamp;
				recordInfo[recordIdx].tableIndex = tableCount;
				tableInfo[tableCount].recordCount++;
				if (LZ4_COMPRESSBOUND(head->size) > blockPageSize - pageSize)
				{
					if (pageSize == 0)
					{
						char * newPage = (char*)malloc(LZ4_COMPRESSBOUND(head->size));
						int size = LZ4_compress(r, newPage, head->size);
						recordInfo[recordIdx].setOffset(pageCount, pageSize);
						writePage(newPage, size);
						free(newPage);
						continue;
					}
					else
					{
						writePage(page, pageSize);
					}
				}
				recordInfo[recordIdx].setOffset(pageCount, pageSize);
				pageSize += LZ4_compress_fast_continue(lz4Stream, r, page + pageSize, head->size, blockPageSize - pageSize, 1);
			} while (aiter.next());
			if (pageSize > 0)
			{
				writePage(page, pageSize);
			}
			tableCount++;
		}
END:
		free(page);
		LZ4_freeStream(lz4Stream);
		return rtv;
	}
	int writeIndex(tableDataInfo * tableInfo, uint64_t &offset)
	{

	}
	int flush()
	{
		remove(m_path.append(".bak").c_str());
		m_fd = openFile(m_path.append(".bak").c_str(), true, true, true);
		uint64_t offset = 0;
		if (!fileHandleValid(m_fd))
		{
			LOG(ERROR) << "open data file :" << m_path.append(".bak") << " failed for errno:" << errno << ",error info:" << strerror(errno);
			return -1;
		}
		int writeSize = 0;
		int headSize = sizeof(block)-offsetof(block, m_version);
		if (headSize != (writeSize = writeFile(m_fd, (char*)&m_version, headSize)))
		{
			LOG(ERROR) << "write head info to block file " << m_path.append(".bak") << " failed for errno:" << errno << " ," << strerror(errno);
			return -1;
		}
		offset = headSize;
		if (headSize > (offset += seekFile(m_fd, sizeof(tableDataInfo)*m_tableCount+sizeof(recordGeneralInfo)*m_recordCount+2*sizeof(dataPartHead), SEEK_CUR)))//from so on, we dont know offset info,so do not write it now
		{
			LOG(ERROR) << "expand  block file " << m_path.append(".bak") << " failed for errno:" << errno << " ," << strerror(errno);
			return -1;
		}
		tableDataInfo * tableInfo = (tableDataInfo*)malloc(sizeof(tableDataInfo)*m_tableCount);
		recordGeneralInfo * recordInfo = (recordGeneralInfo*)malloc(sizeof(recordGeneralInfo)*m_recordCount);
		if( writeIndex(tableInfo)<0)
		{

		}
		if (writeRecordToFile(tableInfo, recordInfo, offset) < 0)
		{
			free(tableInfo);
			free(recordInfo);
			return -1;
		}
	}
};

class appendingBlockLineIterator : public iterator
{
private:
	appendingBlock * m_block;
	uint32_t m_recordID;
	uint32_t m_searchedRecordID;
	appendingBlockLineIterator(appendingBlock * block, filter * filter,uint32_t flag = 0) :iterator(flag, filter),
		m_block(block), m_recordID(0), m_searchedRecordID(0)
	{
		m_block->m_ref.fetch_add(1);
	}
	bool findOffset(uint64_t offset)
	{
		uint64_t recordID = m_block->findRecordIDByOffset(offset);
		if (recordID <= 0)
			return false;
		m_recordID = m_searchedRecordID = recordID;
		m_status = OK;
		return true;
	}
	~appendingBlockLineIterator()
	{
		m_block->m_ref.fetch_sub(1);
	}
	inline const char * value()
	{
		return m_block->m_buf + m_block->m_recordIDs[m_recordID];
	}

	inline bool next()
	{
		do
		{
			if (m_recordID
				>= m_block->m_recordCount)
			{
				if (m_block->m_status != OK)
				{
					if(m_block->m_status == appendingBlock::B_FULL)
						m_status =  ENDED;
					else
						m_status = INVALID;
				}
				else
				{
					m_status = BLOCKED;
					if(m_flag&ITER_FLAG_WAIT)
					m_block->m_cond->m_waiting.push(m_id);
				}
            }
            
        } while (m_filter->doFilter(value()));
        return true;
    }
};
class appendingBlockIterator: public iterator
{
private:
    appendingBlock * m_block;
    std::atomic<uint32_t> m_offset;
    filter * m_filter;
    uint32_t m_id;
    appendingBlockIterator(appendingBlock * block, filter * filter,
            uint32_t offset = 0,uint32_t flag=0) :iterator(flag),
            m_block(block), m_filter(filter), m_offset(offset), m_id(0)
    {
        m_block->m_ref.fetch_add(1);
        assert(
                m_offset.load(std::memory_order_relaxed)
                        <= m_block->m_offset.load(std::memory_order_release));
    }
    ~appendingBlockIterator()
    {
        m_block->m_ref.fetch_sub(1);
    }
    inline void* value() const
    {
        return m_block->m_buf + m_offset.load(std::memory_order_relaxed);
    }
    inline bool next()
    {
        do
        {
            if (m_offset.load(std::memory_order_relaxed)
                    == m_block->m_offset.load(std::memory_order_release))
            {
                if (m_block->m_status != OK)
                    return m_block->m_status;
                m_block->m_cond->m_waiting.push(m_id);
            }
            assert(
                    m_offset.load(std::memory_order_relaxed)
                            < m_block->m_offset.load(
                                    std::memory_order_relaxed));
            m_offset += ((DATABASE_INCREASE::recordHead*) (m_block->m_buf
                    + m_offset.load(std::memory_order_relaxed)))->size;
        } while (m_filter->doFilter(value()));
        return true;
    }
    inline bool end()
    {

    }
    inline void wait()
    {

    }
    inline bool valid()
    {

    }
};
}

