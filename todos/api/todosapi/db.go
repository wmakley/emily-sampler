package todosapi

import (
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

var (
	db *gorm.DB
)

func ConnectToDB(sqliteFileName string) error {
	var err error
	db, err = gorm.Open(sqlite.Open(sqliteFileName), &gorm.Config{})
	return err
}

func AutoMigrate() error {
	return db.AutoMigrate(&Todo{})
}
