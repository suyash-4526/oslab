#!/bin/bash
# Practical 1: Address Book using Shell Programming

FILE="addressbook.txt"

while true
do
    echo
    echo "----- ADDRESS BOOK -----"
    echo "1. Create"
    echo "2. View"
    echo "3. Insert"
    echo "4. Delete"
    echo "5. Modify"
    echo "6. Exit"
    read -rp "Enter choice: " ch

    case "$ch" in
        1)
            touch "$FILE"
            echo "Address book created: $FILE"
            ;;
        2)
            if [[ -s "$FILE" ]]; then
                echo "Name | Phone | Email"
                cat "$FILE"
            else
                echo "Address book is empty or not created."
            fi
            ;;
        3)
            read -rp "Enter name: " name
            read -rp "Enter phone: " phone
            read -rp "Enter email: " email
            printf '%s|%s|%s\n' "$name" "$phone" "$email" >> "$FILE"
            echo "Record inserted."
            ;;
        4)
            read -rp "Enter name to delete: " name
            if [[ -f "$FILE" ]] && grep -Fq "$name|" "$FILE"; then
                grep -Fv "$name|" "$FILE" > "$FILE.tmp"
                mv "$FILE.tmp" "$FILE"
                echo "Record deleted."
            else
                echo "Record not found."
            fi
            ;;
        5)
            read -rp "Enter name to modify: " oldname
            if [[ -f "$FILE" ]] && grep -Fq "$oldname|" "$FILE"; then
                read -rp "Enter new name: " name
                read -rp "Enter new phone: " phone
                read -rp "Enter new email: " email
                grep -Fv "$oldname|" "$FILE" > "$FILE.tmp"
                printf '%s|%s|%s\n' "$name" "$phone" "$email" >> "$FILE.tmp"
                mv "$FILE.tmp" "$FILE"
                echo "Record modified."
            else
                echo "Record not found."
            fi
            ;;
        6)
            echo "Exiting Address Book."
            break
            ;;
        *)
            echo "Invalid choice."
            ;;
    esac
done
