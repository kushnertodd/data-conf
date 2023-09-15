#include "bdb_dao.hpp"
#include "account_dto.hpp"
#include "account_dao.hpp"

/*!
 * @brief load account database from delimited record file
 * @param account_db account database to which to save
 * @param text_file account records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Account_DAO::load(Bdb_dbp &account_db,
                      Bdb_dbp &account_bdb_triplet_db,
                      const std::string &text_file,
                      Bdb_errors &errors,
                      char delimiter) {
  return Bdb_DAO::load_triplets<Account_DTO_key, Account_DTO>
      (account_db,
       account_bdb_triplet_db,
       text_file,
       errors,
       delimiter);
}

/*!
 * @brief select account from database based on account account_id
 * @param account_db account database to which to save
 * @param account_id account record key
 * @param account_dto account dto to save
 * @param errors if save fails
 */
void Account_DAO::lookup(Bdb_dbp &account_db,
                         const std::string &account_id,
                         Account_DTO &account_dto,
                         Bdb_errors &errors) {
  Account_DTO_key account_dto_key(account_id);
  Bdb_DAO::lookup<Account_DTO_key, Account_DTO>
      (account_db, account_dto_key, account_dto, errors);
}

/*!
 * @brief save account dto to account database
 * @param account_db account database to which to save
 * @param account_dto prinicpals dto to save
 * @param errors if save fails
 */
void Account_DAO::save(Bdb_dbp &account_db,
                       Account_DTO &account_dto,
                       Bdb_errors &errors) {
  Account_DTO_key account_dto_key(account_dto);
  Bdb_DAO::save<Account_DTO_key, Account_DTO>
      (account_db, account_dto_key, account_dto, errors);
}

/*!
 * @brief select all account dto records from account database
 * @param account_db account database to select from
 * @param account_dto_list all account dtos in database
 * @param errors if select fails
 */
void Account_DAO::select_all(Bdb_dbp &account_db,
                             Account_DTO_list &account_dto_list,
                             Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(account_db, errors);
  if (!errors.has())
    bdb_cursor.dto_get_list<Account_DTO_key, Account_DTO, Account_DTO_list>
        (account_dto_list, errors);
}

void Account_DAO::update(Bdb_dbp &account_db,
                         Account_DTO &account_dto,
                         Bdb_errors &errors) {
  Account_DTO_key account_dto_key(account_dto);
  Bdb_DAO::save<Account_DTO_key, Account_DTO>
      (account_db, account_dto_key, account_dto, errors);
}

/*!
 * @brief select account key list using account account_id to search account account_id->account key secondary database
 * @param account_account_id_sdb account account_id->account key secondary database
 * @param account_id secondary database search key
 * @param account_dto_key_list selected account key list
 * @param errors if account key not found
 */
void Account_DAO::select_all_for_email(Bdb_dbp &account_email_sdb,
                                       Bdb_dbp &account_db,
                                       const std::string &email,
                                       Account_DTO_list &account_dto_list,
                                       Bdb_errors &errors) {
  Account_email_DTO_key account_email_dto_key(email);
  Account_DTO_key_list account_dto_key_list;
  Bdb_cursor bdb_cursor(account_email_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Account_email_DTO_key,
                                      Account_DTO_key,
                                      Account_DTO_key_list>
        (account_email_dto_key,
         account_dto_key_list,
         errors);
  Bdb_DAO::select_by_key_list<Account_DTO_key,
                              Account_DTO_key_list,
                              Account_DTO,
                              Account_DTO_list>
      (account_db,
       account_dto_key_list,
       account_dto_list,
       errors);
}

/*!
 * @brief select account key list using account account_id to search account account_id->account key secondary database
 * @param account_account_id_sdb account account_id->account key secondary database
 * @param account_id secondary database search key
 * @param account_dto_key_list selected account key list
 * @param errors if account key not found
 */
void Account_DAO::select_all_for_username(Bdb_dbp &account_username_sdb,
                                          Bdb_dbp &account_db,
                                          const std::string &email,
                                          Account_DTO_list &account_dto_list,
                                          Bdb_errors &errors) {
  Account_username_DTO_key account_username_dto_key(email);
  Account_DTO_key_list account_dto_key_list;
  Bdb_cursor bdb_cursor(account_username_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Account_username_DTO_key,
                                      Account_DTO_key,
                                      Account_DTO_key_list>
        (account_username_dto_key,
         account_dto_key_list,
         errors);
  Bdb_DAO::select_by_key_list<Account_DTO_key,
                              Account_DTO_key_list,
                              Account_DTO,
                              Account_DTO_list>
      (account_db,
       account_dto_key_list,
       account_dto_list,
       errors);
}

