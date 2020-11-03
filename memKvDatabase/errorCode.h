#pragma once
namespace KVDB {
	enum errorCode {
		FIELD_NOT_FOUND,
		GET_LOCK_FAILED,
		DATABASE_NOT_EXIST,
		DATABASE_EXIST,
		TABLE_NOT_EXIST,
		TABLE_EXIST,
		ROW_NOT_EXIST,
		ROW_EXIST,
		PRIMARYKEY_NOT_EXIST,
		PRIMARYKEY_NOT_SUPPORT_COLUMN_TYPE,
		COLUMN_COUNT_NOT_MATCH,
		COLUMN_IS_NOT_NULLABLE,
		KEY_COLUMN_NOT_MATCH,
		TRANS_ISOLATION_LEVEL_NOT_SUPPORT,
		TRANSACTION_NOT_START
	};
}