.pragma library
.import Protobuf 1.0 as Protobuf
.import 'NameData.pb.js' as Q__NameData__
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc && Q__NameData__._file.descriptor) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('ChBTdGF0aWNEYXRhLnByb3RvEgpmYW50YXN5Yml0Gg5OYW1lRGF0YS5wcm90byI7CgpQbGF5ZXJCYXNlEg0KBWZpcnN0GB4gASgJEgwKBGxhc3QYKCABKAkSEAoIcG9zaXRpb24YMiABKAkiQAoIR2FtZUluZm8SCgoCaWQYCiABKAkSDAoEaG9tZRgUIAEoCRIMCgRhd2F5GB4gASgJEgwKBHRpbWUYKCABKA0iQwoOV2Vla2x5U2NoZWR1bGUSIwoFZ2FtZXMYCiADKAsyFC5mYW50YXN5Yml0LkdhbWVJbmZvEgwKBGJ5ZXMYFCADKAkiIAoGS3N0YXRzEgoKAnBhGAogASgFEgoKAmZnGBQgAygFIncKBkRzdGF0cxINCgV0d29wdBhkIAEoBRINCgVvbmVwdBhuIAEoBRINCgVkZWZ0ZBh4IAEoBRINCgRzZnR5GIIBIAEoBRISCgl0dXJub3ZlcnMYjAEgASgFEg4KBXNhY2tzGJYBIAEoBRINCgRwdHNhGKABIAEoBSKyAQoGT3N0YXRzEg8KB3Bhc3N5ZHMYCiABKAUSDgoGcGFzc3RkGBQgASgFEg8KB3J1c2h5ZHMYHiABKAUSDgoGcnVzaHRkGCggASgFEg4KBnJlY3lkcxgyIAEoBRINCgVyZWN0ZBg8IAEoBRILCgNyZWMYRiABKAUSDAoEcGludBhQIAEoBRIOCgZmdW1ibGUYWiABKAUSDQoFdHdvcHQYZCABKAUSDQoFb25lcHQYbiABKAUicwoFU3RhdHMSIgoGb3N0YXRzGAogASgLMhIuZmFudGFzeWJpdC5Pc3RhdHMSIgoGZHN0YXRzGBQgASgLMhIuZmFudGFzeWJpdC5Ec3RhdHMSIgoGa3N0YXRzGB4gASgLMhIuZmFudGFzeWJpdC5Lc3RhdHMiuQEKDFBsYXllclJlc3VsdBIQCghwbGF5ZXJpZBgUIAEoCRIOCgZyZXN1bHQYKCABKAISIAoFc3RhdHMYMiABKAsyES5mYW50YXN5Yml0LlN0YXRzEjMKDmZhbnRheWJpdGF3YXJkGDwgAygLMhsuZmFudGFzeWJpdC5GYW50YXN5Qml0QXdhcmQSMAoNZmFudGFzeWJpdHBubBhGIAMoCzIZLmZhbnRhc3liaXQuRmFudGFzeUJpdFBubCKPAQoKR2FtZVJlc3VsdBIOCgZnYW1laWQYASABKAkSLQoLaG9tZV9yZXN1bHQYCiADKAsyGC5mYW50YXN5Yml0LlBsYXllclJlc3VsdBItCgthd2F5X3Jlc3VsdBgUIAMoCzIYLmZhbnRhc3liaXQuUGxheWVyUmVzdWx0EhMKC2tpY2tvZmZ0aW1lGB4gASgN', 1029);
    }
    if (!this._desc)
      console.warn('Failed to initialize: StaticData.proto');
    return this._desc;
  },
};

var PlayerBase = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerBase) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(0));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setFirst(rawArray[0][0]);
      this.setLast(rawArray[0][1]);
      this.setPosition(rawArray[0][2]);
    }
  };

  type.prototype.getFirst = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setFirst = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearFirst = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'first', {
    get: type.prototype.getFirst,
    set: type.prototype.setFirst,
  });

  type.prototype.getLast = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setLast = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearLast = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'last', {
    get: type.prototype.getLast,
    set: type.prototype.setLast,
  });

  type.prototype.getPosition = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setPosition = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPosition = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'position', {
    get: type.prototype.getPosition,
    set: type.prototype.setPosition,
  });

  return type;
})();

var GameInfo = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameInfo) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(1));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setId(rawArray[0][0]);
      this.setHome(rawArray[0][1]);
      this.setAway(rawArray[0][2]);
      this.setTime(rawArray[0][3]);
    }
  };

  type.prototype.getId = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setId = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearId = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'id', {
    get: type.prototype.getId,
    set: type.prototype.setId,
  });

  type.prototype.getHome = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setHome = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearHome = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'home', {
    get: type.prototype.getHome,
    set: type.prototype.setHome,
  });

  type.prototype.getAway = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setAway = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearAway = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'away', {
    get: type.prototype.getAway,
    set: type.prototype.setAway,
  });

  type.prototype.getTime = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setTime = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearTime = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'time', {
    get: type.prototype.getTime,
    set: type.prototype.setTime,
  });

  return type;
})();

var WeeklySchedule = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._games = new Array();
  this._fields[0] = new Array();
  if (values && values.games && values.games instanceof Array) {
    this.games(values.games);
  }
  this._fields[1] = new Array();
  if (values && values.byes && values.byes instanceof Array) {
    this.byes(values.byes);
  }
    if (values instanceof WeeklySchedule) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(2));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      field = rawArray[0][0];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._games[i] == 'undefined') {
            var msg = new GameInfo();
            this._games[i] = msg;
            this._fields[0][i] = msg._raw;
          }
          this._games[i]._mergeFromRawArray(field[i]);
        }
      }
      if (rawArray[0][1] instanceof Array) {
        this.byes(rawArray[0][1]);
      }
    }
  };

    // Replacement setter
  type.prototype.setGames = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[0].length = values.length;
     this._games.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new GameInfo(values[i]);
       this._games[i] = msg;
       this._fields[0][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setGamesAt = function(index, value) {
    var msg = this._maybeConvertToMessage(GameInfo, value);
    this._games[index] = msg;
    this._fields[0][index] = msg._raw;
  };
  // Getter
  type.prototype.getGamesAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[0].length < index) {
      throw new RangeError();
    }
    return this._games[index];
  };
  type.prototype.getGamesAsArray = function() {
    return this._games.slice();
  };
  type.prototype.games = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getGamesAt(indexOrValues);
      } else {
        this.setGamesAt(indexOrValues, value);
      }
    } else {
      this.setGames(indexOrValues);
    }
  };
  var gamesCount = function() {
  console.assert(this._games.length == this._fields[0].length);
    return this._games.length;
  };
  Object.defineProperties(type.prototype, {
    gamesCount: { get: gamesCount },
    gamesSize: { get: gamesCount },
    gamesLength: { get: gamesCount },
  });
  type.prototype.getGamesCount = gamesCount;
  type.prototype.getGamesSize = gamesCount;
  type.prototype.getGamesLength = gamesCount;
  type.prototype.addGames = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(GameInfo, value);
    this._games.push(msg);
    this._fields[0].push(msg._raw);
  console.assert(this._games.length == this._fields[0].length);
  };
  type.prototype.removeGames = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[0].splice(index, 1);
    this._games.splice(index, 1);
  console.assert(this._games.length == this._fields[0].length);
  };
  type.prototype.clearGames = function() {
    this._fields[0].length = 0;
    this._games.length = 0;
  console.assert(this._games.length == this._fields[0].length);
  };
    // Replacement setter
  type.prototype.setByes = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[1] = values.slice();
  };
  // Single value setter
  type.prototype.setByesAt = function(index, value) {
    this._fields[1][index] = value;
  };
  // Getter
  type.prototype.getByesAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._fields[1][index];
  };
  type.prototype.getByesAsArray = function() {
    return this._fields[1].slice();
  };
  type.prototype.byes = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getByesAt(indexOrValues);
      } else {
        this.setByesAt(indexOrValues, value);
      }
    } else {
      this.setByes(indexOrValues);
    }
  };
  var byesCount = function() {
    return this._fields[1].length;
  };
  Object.defineProperties(type.prototype, {
    byesCount: { get: byesCount },
    byesSize: { get: byesCount },
    byesLength: { get: byesCount },
  });
  type.prototype.getByesCount = byesCount;
  type.prototype.getByesSize = byesCount;
  type.prototype.getByesLength = byesCount;
  type.prototype.addByes = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[1].push(value);
  };
  type.prototype.resizeByes = function(len) {
    this._fields[1].length = values.length;
  };
  type.prototype.removeByes = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
  };
  type.prototype.clearByes = function() {
    this._fields[1].length = 0;
  };
  return type;
})();

var Kstats = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._fields[1] = new Int32Array();
  if (values && values.fg && values.fg instanceof Array) {
    this.fg(values.fg);
  }
    if (values instanceof Kstats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(3));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPa(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array || field instanceof Int32Array || field instanceof ArrayBuffer) {
        this.setFg(field);
      }
    }
  };

  type.prototype.getPa = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setPa = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPa = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'pa', {
    get: type.prototype.getPa,
    set: type.prototype.setPa,
  });

    // Replacement setter
  type.prototype.setFg = function(values) {
      if (!values.BYTES_PER_ELEMENT) {
        values = new Int32Array(values);;
      }
      var newArray = this._ensureLength(this._fields[1], values.length);
      if (newArray) {
        this._fields[1] = newArray;
      }
      this._fields[1].set(values);
  };
  // Single value setter
  type.prototype.setFgAt = function(index, value) {
    this._fields[1][index] = value;
  };
  // Getter
  type.prototype.getFgAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._fields[1][index];
  };
  type.prototype.getFgAsArray = function() {
    var array = [];
    for (var i = 0; i < this._fields[1].length; ++i) {
      array.push(this._fields[1][i]);
    }
    return array;
  };
  type.prototype.fg = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getFgAt(indexOrValues);
      } else {
        this.setFgAt(indexOrValues, value);
      }
    } else {
      this.setFg(indexOrValues);
    }
  };
  var fgCount = function() {
    return this._fields[1].length;
  };
  Object.defineProperties(type.prototype, {
    fgCount: { get: fgCount },
    fgSize: { get: fgCount },
    fgLength: { get: fgCount },
  });
  type.prototype.getFgCount = fgCount;
  type.prototype.getFgSize = fgCount;
  type.prototype.getFgLength = fgCount;
  type.prototype.addFg = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var newArray = this._ensureLength(this._fields[1]);
    if (newArray) {
      this._fields[1] = newArray;
    }
    this._fields[1][this._fields[1].length - 1] = value;
  };
  type.prototype.reserveFg = function(len) {
    var blen = len * Int32Array.BYTES_PER_ELEMENT;
    if (this._fields[1].buffer.byteLength < blen) {
      var buf = new ArrayBuffer(blen);
      var newArray = new Int32Array(buf, 0, this._fields[1].length);
      newArray.set(this._fields[1]);
      this._fields[1] = newArray;
    }
  };
  type.prototype.resizeFg = function(len) {
    var newArray = this._ensureLength(this._fields[1], len);
    if (newArray) {
      newArray.set(this._fields[1]);
      this._fields[1] = newArray;
    }
    console.assert(this._fields[1].length >= len);
  };
  type.prototype.removeFg = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
  };
  type.prototype.clearFg = function() {
    this._fields[1] = new Int32Array(this._fields[1].buffer, 0, 0);
  };
  return type;
})();

var Dstats = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Dstats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(4));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTwopt(rawArray[0][0]);
      this.setOnept(rawArray[0][1]);
      this.setDeftd(rawArray[0][2]);
      this.setSfty(rawArray[0][3]);
      this.setTurnovers(rawArray[0][4]);
      this.setSacks(rawArray[0][5]);
      this.setPtsa(rawArray[0][6]);
    }
  };

  type.prototype.getTwopt = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setTwopt = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTwopt = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'twopt', {
    get: type.prototype.getTwopt,
    set: type.prototype.setTwopt,
  });

  type.prototype.getOnept = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setOnept = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearOnept = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'onept', {
    get: type.prototype.getOnept,
    set: type.prototype.setOnept,
  });

  type.prototype.getDeftd = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setDeftd = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearDeftd = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'deftd', {
    get: type.prototype.getDeftd,
    set: type.prototype.setDeftd,
  });

  type.prototype.getSfty = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setSfty = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearSfty = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'sfty', {
    get: type.prototype.getSfty,
    set: type.prototype.setSfty,
  });

  type.prototype.getTurnovers = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setTurnovers = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearTurnovers = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'turnovers', {
    get: type.prototype.getTurnovers,
    set: type.prototype.setTurnovers,
  });

  type.prototype.getSacks = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setSacks = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearSacks = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'sacks', {
    get: type.prototype.getSacks,
    set: type.prototype.setSacks,
  });

  type.prototype.getPtsa = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setPtsa = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearPtsa = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'ptsa', {
    get: type.prototype.getPtsa,
    set: type.prototype.setPtsa,
  });

  return type;
})();

var Ostats = (function() {
  var type = function(values) {
    this._fields = new Array(11);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Ostats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(5));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPassyds(rawArray[0][0]);
      this.setPasstd(rawArray[0][1]);
      this.setRushyds(rawArray[0][2]);
      this.setRushtd(rawArray[0][3]);
      this.setRecyds(rawArray[0][4]);
      this.setRectd(rawArray[0][5]);
      this.setRec(rawArray[0][6]);
      this.setPint(rawArray[0][7]);
      this.setFumble(rawArray[0][8]);
      this.setTwopt(rawArray[0][9]);
      this.setOnept(rawArray[0][10]);
    }
  };

  type.prototype.getPassyds = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setPassyds = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPassyds = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'passyds', {
    get: type.prototype.getPassyds,
    set: type.prototype.setPassyds,
  });

  type.prototype.getPasstd = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setPasstd = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPasstd = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'passtd', {
    get: type.prototype.getPasstd,
    set: type.prototype.setPasstd,
  });

  type.prototype.getRushyds = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setRushyds = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearRushyds = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'rushyds', {
    get: type.prototype.getRushyds,
    set: type.prototype.setRushyds,
  });

  type.prototype.getRushtd = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setRushtd = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearRushtd = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'rushtd', {
    get: type.prototype.getRushtd,
    set: type.prototype.setRushtd,
  });

  type.prototype.getRecyds = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setRecyds = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearRecyds = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'recyds', {
    get: type.prototype.getRecyds,
    set: type.prototype.setRecyds,
  });

  type.prototype.getRectd = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setRectd = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearRectd = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'rectd', {
    get: type.prototype.getRectd,
    set: type.prototype.setRectd,
  });

  type.prototype.getRec = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setRec = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearRec = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'rec', {
    get: type.prototype.getRec,
    set: type.prototype.setRec,
  });

  type.prototype.getPint = function() {
    return typeof this._fields[7] == 'undefined' ? 0 : this._fields[7];
  }
  type.prototype.setPint = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearPint = function() {
    this._fields[7] = 0;
  };
  Object.defineProperty(type.prototype, 'pint', {
    get: type.prototype.getPint,
    set: type.prototype.setPint,
  });

  type.prototype.getFumble = function() {
    return typeof this._fields[8] == 'undefined' ? 0 : this._fields[8];
  }
  type.prototype.setFumble = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearFumble = function() {
    this._fields[8] = 0;
  };
  Object.defineProperty(type.prototype, 'fumble', {
    get: type.prototype.getFumble,
    set: type.prototype.setFumble,
  });

  type.prototype.getTwopt = function() {
    return typeof this._fields[9] == 'undefined' ? 0 : this._fields[9];
  }
  type.prototype.setTwopt = function(value) {
      this._fields[9] = value;
  };
  type.prototype.clearTwopt = function() {
    this._fields[9] = 0;
  };
  Object.defineProperty(type.prototype, 'twopt', {
    get: type.prototype.getTwopt,
    set: type.prototype.setTwopt,
  });

  type.prototype.getOnept = function() {
    return typeof this._fields[10] == 'undefined' ? 0 : this._fields[10];
  }
  type.prototype.setOnept = function(value) {
      this._fields[10] = value;
  };
  type.prototype.clearOnept = function() {
    this._fields[10] = 0;
  };
  Object.defineProperty(type.prototype, 'onept', {
    get: type.prototype.getOnept,
    set: type.prototype.setOnept,
  });

  return type;
})();

var Stats = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Stats) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(6));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.ostats == 'undefined') {
        this.setOstats({});
      }
      this._ostats._mergeFromRawArray(rawArray[0][0]);
      if (typeof this.dstats == 'undefined') {
        this.setDstats({});
      }
      this._dstats._mergeFromRawArray(rawArray[0][1]);
      if (typeof this.kstats == 'undefined') {
        this.setKstats({});
      }
      this._kstats._mergeFromRawArray(rawArray[0][2]);
    }
  };

  type.prototype.getOstats = function() {
    return this._ostats;
  }
  type.prototype.setOstats = function(value) {
    var msg = this._maybeConvertToMessage(Ostats, value);
    this._ostats = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearOstats = function() {
    this._fields[0] = undefined;
    this._ostats = undefined;
  };
  Object.defineProperty(type.prototype, 'ostats', {
    get: type.prototype.getOstats,
    set: type.prototype.setOstats,
  });

  type.prototype.getDstats = function() {
    return this._dstats;
  }
  type.prototype.setDstats = function(value) {
    var msg = this._maybeConvertToMessage(Dstats, value);
    this._dstats = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearDstats = function() {
    this._fields[1] = undefined;
    this._dstats = undefined;
  };
  Object.defineProperty(type.prototype, 'dstats', {
    get: type.prototype.getDstats,
    set: type.prototype.setDstats,
  });

  type.prototype.getKstats = function() {
    return this._kstats;
  }
  type.prototype.setKstats = function(value) {
    var msg = this._maybeConvertToMessage(Kstats, value);
    this._kstats = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearKstats = function() {
    this._fields[2] = undefined;
    this._kstats = undefined;
  };
  Object.defineProperty(type.prototype, 'kstats', {
    get: type.prototype.getKstats,
    set: type.prototype.setKstats,
  });

  return type;
})();

var PlayerResult = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._fantaybitaward = new Array();
  this._fields[3] = new Array();
  if (values && values.fantaybitaward && values.fantaybitaward instanceof Array) {
    this.fantaybitaward(values.fantaybitaward);
  }
  this._fantasybitpnl = new Array();
  this._fields[4] = new Array();
  if (values && values.fantasybitpnl && values.fantasybitpnl instanceof Array) {
    this.fantasybitpnl(values.fantasybitpnl);
  }
    if (values instanceof PlayerResult) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(7));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPlayerid(rawArray[0][0]);
      this.setResult(rawArray[0][1]);
      if (typeof this.stats == 'undefined') {
        this.setStats({});
      }
      this._stats._mergeFromRawArray(rawArray[0][2]);
      field = rawArray[0][3];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._fantaybitaward[i] == 'undefined') {
            var msg = new Q__NameData__.FantasyBitAward();
            this._fantaybitaward[i] = msg;
            this._fields[3][i] = msg._raw;
          }
          this._fantaybitaward[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][4];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._fantasybitpnl[i] == 'undefined') {
            var msg = new Q__NameData__.FantasyBitPnl();
            this._fantasybitpnl[i] = msg;
            this._fields[4][i] = msg._raw;
          }
          this._fantasybitpnl[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getPlayerid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getResult = function() {
    return typeof this._fields[1] == 'undefined' ? 0.000000 : this._fields[1];
  }
  type.prototype.setResult = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearResult = function() {
    this._fields[1] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'result', {
    get: type.prototype.getResult,
    set: type.prototype.setResult,
  });

  type.prototype.getStats = function() {
    return this._stats;
  }
  type.prototype.setStats = function(value) {
    var msg = this._maybeConvertToMessage(Stats, value);
    this._stats = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearStats = function() {
    this._fields[2] = undefined;
    this._stats = undefined;
  };
  Object.defineProperty(type.prototype, 'stats', {
    get: type.prototype.getStats,
    set: type.prototype.setStats,
  });

    // Replacement setter
  type.prototype.setFantaybitaward = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[3].length = values.length;
     this._fantaybitaward.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new Q__NameData__.FantasyBitAward(values[i]);
       this._fantaybitaward[i] = msg;
       this._fields[3][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setFantaybitawardAt = function(index, value) {
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyBitAward, value);
    this._fantaybitaward[index] = msg;
    this._fields[3][index] = msg._raw;
  };
  // Getter
  type.prototype.getFantaybitawardAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[3].length < index) {
      throw new RangeError();
    }
    return this._fantaybitaward[index];
  };
  type.prototype.getFantaybitawardAsArray = function() {
    return this._fantaybitaward.slice();
  };
  type.prototype.fantaybitaward = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getFantaybitawardAt(indexOrValues);
      } else {
        this.setFantaybitawardAt(indexOrValues, value);
      }
    } else {
      this.setFantaybitaward(indexOrValues);
    }
  };
  var fantaybitawardCount = function() {
  console.assert(this._fantaybitaward.length == this._fields[3].length);
    return this._fantaybitaward.length;
  };
  Object.defineProperties(type.prototype, {
    fantaybitawardCount: { get: fantaybitawardCount },
    fantaybitawardSize: { get: fantaybitawardCount },
    fantaybitawardLength: { get: fantaybitawardCount },
  });
  type.prototype.getFantaybitawardCount = fantaybitawardCount;
  type.prototype.getFantaybitawardSize = fantaybitawardCount;
  type.prototype.getFantaybitawardLength = fantaybitawardCount;
  type.prototype.addFantaybitaward = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyBitAward, value);
    this._fantaybitaward.push(msg);
    this._fields[3].push(msg._raw);
  console.assert(this._fantaybitaward.length == this._fields[3].length);
  };
  type.prototype.removeFantaybitaward = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[3].splice(index, 1);
    this._fantaybitaward.splice(index, 1);
  console.assert(this._fantaybitaward.length == this._fields[3].length);
  };
  type.prototype.clearFantaybitaward = function() {
    this._fields[3].length = 0;
    this._fantaybitaward.length = 0;
  console.assert(this._fantaybitaward.length == this._fields[3].length);
  };
    // Replacement setter
  type.prototype.setFantasybitpnl = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[4].length = values.length;
     this._fantasybitpnl.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new Q__NameData__.FantasyBitPnl(values[i]);
       this._fantasybitpnl[i] = msg;
       this._fields[4][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setFantasybitpnlAt = function(index, value) {
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyBitPnl, value);
    this._fantasybitpnl[index] = msg;
    this._fields[4][index] = msg._raw;
  };
  // Getter
  type.prototype.getFantasybitpnlAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[4].length < index) {
      throw new RangeError();
    }
    return this._fantasybitpnl[index];
  };
  type.prototype.getFantasybitpnlAsArray = function() {
    return this._fantasybitpnl.slice();
  };
  type.prototype.fantasybitpnl = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getFantasybitpnlAt(indexOrValues);
      } else {
        this.setFantasybitpnlAt(indexOrValues, value);
      }
    } else {
      this.setFantasybitpnl(indexOrValues);
    }
  };
  var fantasybitpnlCount = function() {
  console.assert(this._fantasybitpnl.length == this._fields[4].length);
    return this._fantasybitpnl.length;
  };
  Object.defineProperties(type.prototype, {
    fantasybitpnlCount: { get: fantasybitpnlCount },
    fantasybitpnlSize: { get: fantasybitpnlCount },
    fantasybitpnlLength: { get: fantasybitpnlCount },
  });
  type.prototype.getFantasybitpnlCount = fantasybitpnlCount;
  type.prototype.getFantasybitpnlSize = fantasybitpnlCount;
  type.prototype.getFantasybitpnlLength = fantasybitpnlCount;
  type.prototype.addFantasybitpnl = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyBitPnl, value);
    this._fantasybitpnl.push(msg);
    this._fields[4].push(msg._raw);
  console.assert(this._fantasybitpnl.length == this._fields[4].length);
  };
  type.prototype.removeFantasybitpnl = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[4].splice(index, 1);
    this._fantasybitpnl.splice(index, 1);
  console.assert(this._fantasybitpnl.length == this._fields[4].length);
  };
  type.prototype.clearFantasybitpnl = function() {
    this._fields[4].length = 0;
    this._fantasybitpnl.length = 0;
  console.assert(this._fantasybitpnl.length == this._fields[4].length);
  };
  return type;
})();

var GameResult = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._homeResult = new Array();
  this._fields[1] = new Array();
  if (values && values.homeResult && values.homeResult instanceof Array) {
    this.homeResult(values.homeResult);
  }
  this._awayResult = new Array();
  this._fields[2] = new Array();
  if (values && values.awayResult && values.awayResult instanceof Array) {
    this.awayResult(values.awayResult);
  }
    if (values instanceof GameResult) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(8));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setGameid(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._homeResult[i] == 'undefined') {
            var msg = new PlayerResult();
            this._homeResult[i] = msg;
            this._fields[1][i] = msg._raw;
          }
          this._homeResult[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][2];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._awayResult[i] == 'undefined') {
            var msg = new PlayerResult();
            this._awayResult[i] = msg;
            this._fields[2][i] = msg._raw;
          }
          this._awayResult[i]._mergeFromRawArray(field[i]);
        }
      }
      this.setKickofftime(rawArray[0][3]);
    }
  };

  type.prototype.getGameid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setGameid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearGameid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'gameid', {
    get: type.prototype.getGameid,
    set: type.prototype.setGameid,
  });

    // Replacement setter
  type.prototype.setHomeResult = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[1].length = values.length;
     this._homeResult.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new PlayerResult(values[i]);
       this._homeResult[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setHomeResultAt = function(index, value) {
    var msg = this._maybeConvertToMessage(PlayerResult, value);
    this._homeResult[index] = msg;
    this._fields[1][index] = msg._raw;
  };
  // Getter
  type.prototype.getHomeResultAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._homeResult[index];
  };
  type.prototype.getHomeResultAsArray = function() {
    return this._homeResult.slice();
  };
  type.prototype.homeResult = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getHomeResultAt(indexOrValues);
      } else {
        this.setHomeResultAt(indexOrValues, value);
      }
    } else {
      this.setHomeResult(indexOrValues);
    }
  };
  var homeResultCount = function() {
  console.assert(this._homeResult.length == this._fields[1].length);
    return this._homeResult.length;
  };
  Object.defineProperties(type.prototype, {
    homeResultCount: { get: homeResultCount },
    homeResultSize: { get: homeResultCount },
    homeResultLength: { get: homeResultCount },
  });
  type.prototype.getHomeResultCount = homeResultCount;
  type.prototype.getHomeResultSize = homeResultCount;
  type.prototype.getHomeResultLength = homeResultCount;
  type.prototype.addHomeResult = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(PlayerResult, value);
    this._homeResult.push(msg);
    this._fields[1].push(msg._raw);
  console.assert(this._homeResult.length == this._fields[1].length);
  };
  type.prototype.removeHomeResult = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
    this._homeResult.splice(index, 1);
  console.assert(this._homeResult.length == this._fields[1].length);
  };
  type.prototype.clearHomeResult = function() {
    this._fields[1].length = 0;
    this._homeResult.length = 0;
  console.assert(this._homeResult.length == this._fields[1].length);
  };
    // Replacement setter
  type.prototype.setAwayResult = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[2].length = values.length;
     this._awayResult.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new PlayerResult(values[i]);
       this._awayResult[i] = msg;
       this._fields[2][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setAwayResultAt = function(index, value) {
    var msg = this._maybeConvertToMessage(PlayerResult, value);
    this._awayResult[index] = msg;
    this._fields[2][index] = msg._raw;
  };
  // Getter
  type.prototype.getAwayResultAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[2].length < index) {
      throw new RangeError();
    }
    return this._awayResult[index];
  };
  type.prototype.getAwayResultAsArray = function() {
    return this._awayResult.slice();
  };
  type.prototype.awayResult = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getAwayResultAt(indexOrValues);
      } else {
        this.setAwayResultAt(indexOrValues, value);
      }
    } else {
      this.setAwayResult(indexOrValues);
    }
  };
  var awayResultCount = function() {
  console.assert(this._awayResult.length == this._fields[2].length);
    return this._awayResult.length;
  };
  Object.defineProperties(type.prototype, {
    awayResultCount: { get: awayResultCount },
    awayResultSize: { get: awayResultCount },
    awayResultLength: { get: awayResultCount },
  });
  type.prototype.getAwayResultCount = awayResultCount;
  type.prototype.getAwayResultSize = awayResultCount;
  type.prototype.getAwayResultLength = awayResultCount;
  type.prototype.addAwayResult = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(PlayerResult, value);
    this._awayResult.push(msg);
    this._fields[2].push(msg._raw);
  console.assert(this._awayResult.length == this._fields[2].length);
  };
  type.prototype.removeAwayResult = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[2].splice(index, 1);
    this._awayResult.splice(index, 1);
  console.assert(this._awayResult.length == this._fields[2].length);
  };
  type.prototype.clearAwayResult = function() {
    this._fields[2].length = 0;
    this._awayResult.length = 0;
  console.assert(this._awayResult.length == this._fields[2].length);
  };
  type.prototype.getKickofftime = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setKickofftime = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearKickofftime = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'kickofftime', {
    get: type.prototype.getKickofftime,
    set: type.prototype.setKickofftime,
  });

  return type;
})();

