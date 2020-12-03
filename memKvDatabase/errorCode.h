#pragma once
namespace KVDB {
	enum errorCode {
		INNER_ERROR,
		ILLEGAL_MESSAGE,
		AUTH_FAILED,
		DO_NOT_CONNECT_TO_DATABASE,
		FIELD_NOT_FOUND,
		GET_LOCK_FAILED,
		UNKNOWN_OPERATION_TYPE,
		DATABASE_NOT_EXIST,
		DATABASE_EXIST,
		NO_DATABASE_SELECTED,
		TABLE_NOT_EXIST,
		TABLE_EXIST,
		NO_TABLE_SELECTED,
		ROW_NOT_EXIST,
		ROW_EXIST,
		PRIMARYKEY_NOT_EXIST,
		PRIMARYKEY_NOT_SUPPORT_COLUMN_TYPE,
		COLUMN_COUNT_NOT_MATCH,
		COLUMN_IS_NOT_NULLABLE,
		KEY_COLUMN_NOT_MATCH,
		TRANS_ISOLATION_LEVEL_NOT_SUPPORT,
		TRANSACTION_NOT_START,
		TRANSACTION_HAS_START,
		INVALID_ROW_CHANGE
	};
}