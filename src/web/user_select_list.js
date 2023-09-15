/*
 * requests.js:
 * Request class
 */

class User_Select_list {
    static select_list_name = "select_user";
    static div_id = "select_user_list";
    static id = "select_user_id";

    static select_list;
    static data;
    static first = true;

    static clear() {
        User_Select_list.select_list.clear();
    }


    static create() {
        let request = new Request({
            request: "account_select_all"
        });
        request.send(User_Select_list.select_users_request_success, User_Select_list.select_users_request_failure);
    }

    static init() {
        User_Select_list.select_list = new Select_list({
            name: User_Select_list.select_list_name,
            div_id: User_Select_list.div_id,
            id: User_Select_list.id,
            event_listener: User_Select_list.user_list_selected
        });
        this.create();
    }

    static select_users_request_success(result) {
        let result_obj = Request.parse_response(result);
        let request_name = result_obj.dconf_request.request;
        User_Select_list.user_list_populate(result_obj.dconf_request_response.account_dto_list);
    }

    static select_users_request_failure(req) {
        alert("select user request failed: '" + req + "'");
    }


    static user_list_populate(account_dto_list) {
        let select_list = User_Select_list.select_list;
        select_list.clear();
        select_list.add_option({text: "Select user", disabled: true});
        for (let i = 0; i < account_dto_list.length; i++) {
            let account = account_dto_list[i];
            select_list.add_option({
                data: account,
                text: account.username,
                value: account.account_id
            });
        }
    }

    static user_list_selected(e) {
        if (User_Select_list.first) {
            User_Select_list.first = false;
            User_Select_list.select_list.remove(0);
        }
        let select_list = User_Select_list.select_list.select_list;
        let selected_option = select_list.selectedOptions[0];
        let data = selected_option.data;
        User_Select_list.data = data;
        let account_id = data.account_id;
        User_Select_list.username = data.username;
        let request = new Request({
            request: "order_select_all_for_account_id",
            arguments: account_id
        });
        request.send(Order_table.user_orders_select_request_success, Order_table.user_orders_select_request_failure);
    }

}
