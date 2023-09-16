/*
 * requests.js:
 * Request class
 */

class Order_table {
    static table_name = "order";
    static div_id = "order_table";
    static id = "order_table_id";

    static editing;
    static label;
    static table;
    static selected_row;
    static order_name;
    static order_id;

    static buttons;
    static delete_order_button;
    static edit_order_button;
    static new_order_button;

    static show_items(order_id) {
        let request = new Request({
            request: "order_select_all_items",
            arguments: order_id
        });
        request.send(Item_table.select_order_items_request_success, Item_table.select_order_items_request_failure);
    }

    static button_delete_order(e) {
        e.preventDefault();
        Order_table.editing = false;

        let request = new Request({
            request: "order_remove",
            arguments: Order_table.order_id
        });
        request.send(Order_table.user_orders_delete_request_success, Order_table.user_orders_delete_request_failure);
    }

    static button_edit_order(e) {
        e.preventDefault();
        Order_table.editing = true;
        Order_table.edit_order_button.disable();
        Item_table.editing();
        let order_id = Order_table.order_id;
        let request = new Request({
            request: "order_select_other_items",
            arguments: order_id
        });
        // TODO: somehow disable edit button here
        request.send(Item_pool_table.select_order_other_items_request_success, Item_pool_table.select_order_other_items_request_failure);
    }

    static button_new_order(e) {
        e.preventDefault();
        let account_id = User_Select_list.data.account_id;
        let order_name = prompt("New order name", "");
        if (order_name != null) {
            let request = new Request({
                request: "order_new",
                arguments: [order_name, account_id]
            });
            request.send(Order_table.user_orders_new_request_success, Order_table.user_orders_new_request_failure);
        }
    }

    static clear() {
        Item_table.clear();
        Item_pool_table.clear();
        Order_table.table.clear();
        Order_table.label_unset();
        Order_table.buttons.hide();
    }


    static create(order_list) {
        Order_table.clear();
        let table = Order_table.table;
        table.add_row({id: "r0"})
        let header = "Order";
        table.add_th({
            row_id: "r0",
            id: "h0",
            text: header,
            class_name: " header",
            width: '50%'
        });
        if (order_list.length > 0)
            for (let i = 0; i < order_list.length; i++) {
                let order = order_list[i];
                let row_id = `r${i}`;
                let tr = table.add_row({data: order, id: row_id})
                tr.addEventListener('click', Order_table.row_onlick_handler);
                table.add_td({row_id: row_id, id: "d0", text: order_list[i].name})
            }
        Order_table.label_set();
        Order_table.delete_order_button.disable();
        Order_table.edit_order_button.disable();
        Order_table.new_order_button.enable();
        Order_table.buttons.show();
    }


    static init() {
        Order_table.table = new Table({
            name: Order_table.table_name,
            div_id: Order_table.div_id,
            id: Order_table.id
        });
        Order_table.label = $("#displayed_user");
        Order_table.buttons = new Button_set({
            name: "buttons",
            div_id: "order_table_buttons",
            hidden: true
        });
        Order_table.delete_order_button = new Button({
            name: "Delete order",
            id: "order_table_delete_order_id",
            event_listener: Order_table.button_delete_order,
            disabled: true
        });
        Order_table.buttons.add_button(Order_table.delete_order_button);
        Order_table.edit_order_button = new Button({
            name: "Edit order",
            id: "order_table_edit_order_id",
            event_listener: Order_table.button_edit_order,
            disabled: true
        });
        Order_table.buttons.add_button(Order_table.edit_order_button);
        Order_table.new_order_button = new Button({
            name: "New order",
            id: "order_table_new_order_id",
            event_listener: Order_table.button_new_order,
            disabled: false
        });
        Order_table.buttons.add_button(Order_table.new_order_button);
    }

    static label_set() {
        let username = User_Select_list.data.username;
        Order_table.label.html(`${username} orders:`);
    }

    static label_unset() {
        Order_table.label.html("");
    }

    static row_onlick_handler(e) {
        e.preventDefault();
        let order_row_selected = e.currentTarget;
        Order_table.selected_row = order_row_selected;
        Order_table.table.select_row(order_row_selected);
        let order = order_row_selected.data;
        let order_id = order.order_id;
        Order_table.order_id = order_id;
        Order_table.order_name = order.name;
        Order_table.buttons.show();
        Order_table.delete_order_button.enable();
        Order_table.edit_order_button.enable();
        Order_table.editing = false;
        Order_table.show_items(order_id);
    }


    static user_orders_delete_request_failure(req) {
        alert(`select orders request failed: '${req}'`);
    }

    static user_orders_delete_request_success(result) {
        let data = User_Select_list.data;
        let account_id = data.account_id;
        //User_Select_list.username = data.username;
        let request = new Request({
            request: "order_select_all_for_account_id",
            arguments: account_id
        });
        request.send(Order_table.user_orders_select_request_success, Order_table.user_orders_select_request_failure);

    }

    static user_orders_new_request_failure(req) {
        alert(`new order request failed: '${req}'`);
    }

    static user_orders_new_request_success(result) {
        let data = User_Select_list.data;
        let account_id = data.account_id;
        //User_Select_list.username = data.username;
        let request = new Request({
            request: "order_select_all_for_account_id",
            arguments: account_id
        });
        request.send(Order_table.user_orders_select_request_success, Order_table.user_orders_select_request_failure);

    }

    static user_orders_select_request_failure(req) {
        alert(`select orders request failed: '${req}'`);
    }

    static user_orders_select_request_success(result) {
        let result_obj = Request.parse_response(result);
        let request_name = result_obj.dconf_request.request;
        Order_table.create(result_obj.dconf_request_response.order_dto_list);
    }

}
