const mysql = require('mysql2');

const express = require("express");
const { MongoClient, ObjectId } = require("mongodb");

const app = express();
app.use(express.json());

const mongoURI =
  "mongodb+srv://sjaya6:<db_password>@cluster0.5yjay.mongodb.net/cs480-project2?retryWrites=true&w=majority";

const dbPassword = "SanjithJ@314";
const dbURI = mongoURI.replace("<db_password>", encodeURIComponent(dbPassword));

let db1;
let db2
const client = new MongoClient(dbURI);

(async () => {
  try {
    await client.connect();
    db1 = client.db("cs480-project2");
    db2 = client.db("sample_mflix");
    console.log("Connected to MongoDB Atlas");
  } catch (err) {
    console.error("MongoDB Atlas connection error:", err);
    process.exit(1);
  }
})();

const COLORS_COLLECTION = "colors";
const MOVIES_COLLECTION = "movies";

//Get all colors
app.get("/api/v1/colors", async (req, res) => {
  try {
    const records = await db1.collection(COLORS_COLLECTION).find().toArray();
    res.status(200).send(records);
  } catch (error) {
    res.status(500).send({ error: "Error fetching colors", details: error.message });
  }
});


//Insert one color from the string
app.post("/api/v1/colors", async (req, res) => {
  try {
    const input = JSON.parse(req);
    const result = await db1.collection(COLORS_COLLECTION).insertOne(input);
    res.status(201).send(result);
  } catch (error) {
    res.status(500).send({ error: "Error inserting color", details: error.message });
  }
});

//Get one color with ID
app.get("/api/v1/color/:id", async (req, res) => {
  try {
    const color = await db1.collection(COLORS_COLLECTION).findOne({
      _id: new ObjectId(req.params.id),
    });
    if (!color) return res.status(404).send({ error: "Color not found" });
    res.status(200).send(color);
  } catch (error) {
    res.status(500).send({ error: "Error fetching color", details: error.message });
  }
});

app.put("/api/v1/colors/:id", async (req, res) => {
  try {
    const input = req.body;
    const result = await db1.collection(COLORS_COLLECTION).findOneAndUpdate(
      { _id: new ObjectId(req.params.id) },
      { $set: input },
      { returnDocument: "after" }
    );
    if (!result.value) return res.status(404).send({ error: "Updated Color, value changed in db" });
    res.status(200).send(result.value);
  } catch (error) {
    res.status(500).send({ error: "Error updating color", details: error.message });
  }
});


//Delete a color with ID
app.delete("/api/v1/colors/:id", async (req, res) => {
  try {
    const result = await db1.collection(COLORS_COLLECTION).deleteOne({
      _id: new ObjectId(req.params.id),
    });
    if (result.deletedCount === 0)
      return res.status(404).send({ error: "Color not found" });
    res.status(200).send({ message: "Color deleted" });
  } catch (error) {
    res.status(500).send({ error: "Error deleting color", details: error.message });
  }
});

app.get("/api/v1/movies", async (req, res) => {
  try {
    const filter = {};
    Object.keys(req.query).forEach((key) => {
      filter[key] = req.query[key];
    });

    const movies = await db2
      .collection(MOVIES_COLLECTION)
      .find(filter)
      .limit(10)
      .toArray();

    res.status(200).send(movies);
  } catch (error) {
    res.status(500).send({ error: "Error fetching movies", details: error.message });
  }
});

//MYSQL
const DB_CONFIG = {
    host: '34.133.101.98',
    user: 'remote-admin',
    password: 'pass1234',
    database: 'sakila',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0,
};

const pool = mysql.createPool(DB_CONFIG);

//Get APIs for actors, films, customers and stores
app.get('/api/v1/actors', (req, res) => {
    pool.query('SELECT * FROM actor', (err, results) => {
        if (err) {
            console.error('Database query error:', err);
            res.status(500).send('Database query failed');
            return;
        }
        res.json(results);
    });
});


app.get('/api/v1/films', (req, res) => {
    const searchQuery = req.query.query;

    let sql = 'SELECT * FROM film';
    const values = [];

    if (searchQuery) {
        sql += ' WHERE title = ?';
        const exactQuery = searchQuery.toLowerCase();
        values.push(exactQuery, exactQuery);
    }

    pool.query(sql, values, (err, results) => {
        if (err) {
            console.error('Database query error:', err);
            res.status(500).send('Database query failed');
            return;
        }

        res.json(results);
    });
});


app.get('/api/v1/customers', (req, res) => {
    pool.query('SELECT * FROM customer', (err, results) => {
        if (err) {
            console.error('Database query error:', err);
            res.status(500).send('Database query failed');
            return;
        }
        res.json(results);
    });
});


app.get('/api/v1/stores', (req, res) => {
    pool.query('SELECT * FROM store', (err, results) => {
        if (err) {
            console.error('Database query error:', err);
            res.status(500).send('Database query failed');
            return;
        }
        res.json(results);
    });
});

//Get value by id
app.get('/api/v1/actors/:id', (req, res) => {
  const actorId = req.params.id;

  const sql = 'SELECT * FROM actor WHERE actor_id = ?';

  pool.query(sql, [actorId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Actor not found');
          return;
      }

      res.json(results);
  });
});

app.get('/api/v1/films/:id', (req, res) => {
  const filmId = req.params.id;

  const sql = 'SELECT * FROM film WHERE film_id = ?';

  pool.query(sql, [filmId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }
      res.json(results);
  });
});

app.get('/api/v1/stores/:id', (req, res) => {
  const storeId = req.params.id;

  const sql = 'SELECT * FROM store WHERE store_id = ?';

  pool.query(sql, [storeId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Store not found');
          return;
      }

      res.json(results);
  });
});

app.get('/api/v1/customers/:id', (req, res) => {
  const customerId = req.params.id;

  const sql = 'SELECT * FROM customer WHERE customer_id = ?';

  pool.query(sql, [customerId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Customer not found');
          return;
      }

      res.json(results);
  });
});

app.get('/api/v1/actors/:id/films', (req, res) => {
  const actorId = req.params.id;

  const sql = 'SELECT * FROM film_actor JOIN film ON film_actor.film_id = film.film_id WHERE film_actor.actor_id = ?';

  pool.query(sql, [actorId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      res.json(results);
  });
});

//Check
app.get('/api/v1/films/:id/actors', (req, res) => {
  const filmId = req.params.id;

  const sql = 'SELECT * FROM film_actor JOIN actor ON film_actor.actor_id = actor.actor_id WHERE film_actor.film_id = ?';

  pool.query(sql, [filmId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      res.json(results);
  });
});

app.get('/api/v1/films/:id/detail', (req, res) => {
  const filmId = req.params.id;

  const sql = 'SELECT * FROM film_list WHERE FID = ?';

  pool.query(sql, [filmId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Film not found');
          return;
      }

      res.json(results[0]);
  });
});

//Check
app.get('/api/v1/customers/:id/detail', (req, res) => {
  const customerId = req.params.id;

  const sql = 'SELECT * FROM customer_list WHERE ID = ?';

  pool.query(sql, [customerId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Customer not found');
          return;
      }

      res.json(results[0]);
  });
});

app.get('/api/v1/actors/:id/detail', (req, res) => {
  const actorId = req.params.id;

  const sql = 'SELECT * FROM actor_info WHERE actor_id = ?';

  pool.query(sql, [actorId], (err, results) => {
      if (err) {
          console.error('Database query error:', err);
          res.status(500).send('Database query failed');
          return;
      }

      if (results.length === 0) {
          res.status(404).send('Actor not found');
          return;
      }

      res.json(results[0]);
  });
});

app.get('/api/v1/inventory-in-stock/:film_id/:store_id', (req, res) => {
    const { film_id, store_id } = req.params;

    const sql = 'CALL film_in_stock(?, ?, @count)';

    pool.query(sql, [film_id, store_id], (err, results) => {
        if (err) {
            console.error('Database query error:', err);
            res.status(500).send('Database query failed');
            return;
        }

        if (results.length === 0 || results[0].length === 0) {
            res.status(404).send('No inventory found for the given film and store');
            return;
        }

        res.json(results[0]);
    });
});            


const PORT = 3000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));