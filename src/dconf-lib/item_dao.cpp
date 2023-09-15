#include "bdb_dao.hpp"
#include "item_dto.hpp"
#include "item_dao.hpp"

/*!
 * @brief load item database from delimited record file
 * @param item_db item database to which to save
 * @param text_file item records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Item_DAO::load(Bdb_dbp &item_db,
                   Bdb_dbp &item_bdb_triplet_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load_triplets<Item_DTO_key,
                                Item_DTO>(item_db,
                                          item_bdb_triplet_db,
                                          text_file,
                                          errors,
                                          delimiter);
}

/*!
 * @brief select item from database based on item id
 * @param item_db item database to which to save
 * @param item_id item record key
 * @param item_dto item dto to save
 * @param errors if save fails
 */
void Item_DAO::lookup(Bdb_dbp &item_db,
                      const std::string &item_id,
                      Item_DTO &item_dto,
                      Bdb_errors &errors) {
  Item_DTO_key item_dto_key(item_id);
  Bdb_DAO::lookup<Item_DTO_key,
                  Item_DTO>(item_db, item_dto_key, item_dto, errors);
}

/*!
 * @brief save item dto to item database
 * @param item_db item database to which to save
 * @param item_dto prinicpals dto to save
 * @param errors if save fails
 */
void Item_DAO::save(Bdb_dbp &item_db, Item_DTO &item_dto, Bdb_errors &errors) {
  Item_DTO_key item_dto_key(item_dto);
  Bdb_DAO::save<Item_DTO_key,
                Item_DTO>(item_db, item_dto_key, item_dto, errors);
}

/*!
 * @brief select all item dto records from item database
 * @param item_db item database to select from
 * @param item_dto_list all item dtos in database
 * @param errors if select fails
 */
void Item_DAO::select_all(Bdb_dbp &item_db,
                          Item_DTO_list &item_dto_list,
                          Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(item_db, errors);
  if (!errors.has())
    bdb_cursor.dto_get_list<Item_DTO_key,
                            Item_DTO,
                            Item_DTO_list>(item_dto_list, errors);
}

/*!
 * @brief select item key list using item id to search item id->item key secondary database
 * @param item_item_id_sdb item id->item key secondary database
 * @param item_id secondary database search key
 * @param item_DTO_key_list selected item key list
 * @param errors if item key not found
 */
void Item_DAO::select_all_for_cost(Bdb_dbp &item_cost_sdb,
                                      Bdb_dbp &item_db,
                                      const std::string &cost,
                                      Item_DTO_list &item_dto_list,
                                      Bdb_errors &errors) {
  Item_DTO_cost_key item_DTO_cost_key(cost);
  Item_DTO_key_list item_dto_key_list;
  Bdb_cursor bdb_cursor(item_cost_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Item_DTO_cost_key,
                                      Item_DTO_key,
                                      Item_DTO_key_list>
        (item_DTO_cost_key,
         item_dto_key_list,
         errors);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Item_DTO_key,
                                Item_DTO_key_list,
                                Item_DTO,
                                Item_DTO_list>
        (item_db,
         item_dto_key_list,
         item_dto_list,
         errors);
}

/*!
 * @brief select item key list using item id to search item id->item key secondary database
 * @param item_item_id_sdb item id->item key secondary database
 * @param item_id secondary database search key
 * @param item_DTO_key_list selected item key list
 * @param errors if item key not found
 */
void Item_DAO::select_all_for_name(Bdb_dbp &item_name_sdb,
                                   Bdb_dbp &item_db,
                                   const std::string &name,
                                   Item_DTO_list &item_dto_list,
                                   Bdb_errors &errors) {
  Item_DTO_name_key item_DTO_name_key(name);
  Item_DTO_key_list item_dto_key_list;
  Bdb_cursor bdb_cursor(item_name_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Item_DTO_name_key,
                                      Item_DTO_key,
                                      Item_DTO_key_list>
        (item_DTO_name_key,
         item_dto_key_list,
         errors);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Item_DTO_key,
                                Item_DTO_key_list,
                                Item_DTO,
                                Item_DTO_list>
        (item_db,
         item_dto_key_list,
         item_dto_list,
         errors);
}

void Item_DAO::update(Bdb_dbp &item_db,
                      Item_DTO &item_dto,
                      Bdb_errors &errors) {
  Item_DTO_key item_dto_key(item_dto);
  Bdb_DAO::save<Item_DTO_key, Item_DTO>
      (item_db, item_dto_key, item_dto, errors);
}
