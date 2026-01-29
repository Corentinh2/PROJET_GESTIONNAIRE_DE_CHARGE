import { useState } from 'react';
import { ArrowLeft, Plug, MapPin, Zap, CheckCircle2, Battery, Plus, X } from 'lucide-react';

interface ChargingStationSelectProps {
  onBack: () => void;
}

interface ChargingStation {
  id: string;
  name: string;
  location: string;
  status: 'available' | 'occupied' | 'maintenance';
  power: string;
  type: string;
  distance: string;
}

const initialStations: ChargingStation[] = [
  {
    id: '1',
    name: 'Borne A - Parking Principal',
    location: 'Niveau 1, Zone A',
    status: 'available',
    power: '22 kW',
    type: 'Type 2',
    distance: '50m'
  },
  {
    id: '2',
    name: 'Borne B - Parking Principal',
    location: 'Niveau 1, Zone A',
    status: 'occupied',
    power: '22 kW',
    type: 'Type 2',
    distance: '55m'
  },
  {
    id: '3',
    name: 'Borne C - Parking Rapide',
    location: 'Extérieur, Zone B',
    status: 'available',
    power: '50 kW',
    type: 'CCS',
    distance: '120m'
  },
  {
    id: '4',
    name: 'Borne D - Parking Rapide',
    location: 'Extérieur, Zone B',
    status: 'available',
    power: '50 kW',
    type: 'CCS',
    distance: '125m'
  },
  {
    id: '5',
    name: 'Borne E - Station Premium',
    location: 'Niveau 2, Zone C',
    status: 'maintenance',
    power: '150 kW',
    type: 'CCS Ultra',
    distance: '200m'
  },
  {
    id: '6',
    name: 'Borne F - Station Premium',
    location: 'Niveau 2, Zone C',
    status: 'available',
    power: '150 kW',
    type: 'CCS Ultra',
    distance: '205m'
  }
];

export function ChargingStationSelect({ onBack }: ChargingStationSelectProps) {
  const [chargingStations, setChargingStations] = useState<ChargingStation[]>(initialStations);
  const [selectedStation, setSelectedStation] = useState<string | null>(null);
  const [filter, setFilter] = useState<'all' | 'available'>('all');
  const [showAddStation, setShowAddStation] = useState(false);
  
  // Form states
  const [formData, setFormData] = useState({
    name: '',
    location: '',
    status: 'available' as 'available' | 'occupied' | 'maintenance',
    power: '',
    type: '',
    distance: ''
  });

  const filteredStations = chargingStations.filter(station => 
    filter === 'all' || station.status === 'available'
  );

  const handleSelectStation = (stationId: string) => {
    const station = chargingStations.find(s => s.id === stationId);
    if (station?.status === 'available') {
      setSelectedStation(stationId);
    }
  };

  const handleStartCharging = () => {
    if (selectedStation) {
      const station = chargingStations.find(s => s.id === selectedStation);
      alert(`Recharge démarrée à la ${station?.name} !`);
    }
  };

  const handleAddStation = (e: React.FormEvent) => {
    e.preventDefault();
    
    const newStation: ChargingStation = {
      id: Date.now().toString(),
      name: formData.name,
      location: formData.location,
      status: formData.status,
      power: formData.power,
      type: formData.type,
      distance: formData.distance
    };

    setChargingStations([...chargingStations, newStation]);
    setShowAddStation(false);
    
    // Reset form
    setFormData({
      name: '',
      location: '',
      status: 'available',
      power: '',
      type: '',
      distance: ''
    });
    
    alert(`Borne ${formData.name} ajoutée avec succès!`);
  };

  const handleInputChange = (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement>) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: value
    }));
  };

  const getStatusColor = (status: string) => {
    switch (status) {
      case 'available':
        return 'bg-green-100 text-green-700 border-green-200';
      case 'occupied':
        return 'bg-red-100 text-red-700 border-red-200';
      case 'maintenance':
        return 'bg-yellow-100 text-yellow-700 border-yellow-200';
      default:
        return 'bg-gray-100 text-gray-700 border-gray-200';
    }
  };

  const getStatusText = (status: string) => {
    switch (status) {
      case 'available':
        return 'Disponible';
      case 'occupied':
        return 'Occupée';
      case 'maintenance':
        return 'Maintenance';
      default:
        return 'Inconnu';
    }
  };

  return (
    <div className="min-h-screen bg-gray-50">
      {/* Header */}
      <header className="bg-white shadow-sm sticky top-0 z-10">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4">
          <div className="flex items-center justify-between">
            <div className="flex items-center gap-4">
              <button
                onClick={onBack}
                className="p-2 hover:bg-gray-100 rounded-lg transition-colors"
              >
                <ArrowLeft className="w-6 h-6 text-gray-700" />
              </button>
              <div>
                <h1 className="text-2xl font-bold text-gray-900">Sélection de Borne</h1>
                <p className="text-sm text-gray-500">Choisissez votre borne de recharge</p>
              </div>
            </div>
            <button
              onClick={() => setShowAddStation(true)}
              className="flex items-center gap-2 px-4 py-2 bg-blue-600 text-white rounded-lg hover:bg-blue-700 transition-colors"
            >
              <Plus className="w-5 h-5" />
              <span className="hidden sm:inline">Ajouter une borne</span>
            </button>
          </div>
        </div>
      </header>

      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        {/* Filter Buttons */}
        <div className="mb-6 flex gap-3">
          <button
            onClick={() => setFilter('all')}
            className={`px-4 py-2 rounded-lg font-medium transition-colors ${
              filter === 'all'
                ? 'bg-blue-600 text-white'
                : 'bg-white text-gray-700 border border-gray-300 hover:bg-gray-50'
            }`}
          >
            Toutes les bornes
          </button>
          <button
            onClick={() => setFilter('available')}
            className={`px-4 py-2 rounded-lg font-medium transition-colors ${
              filter === 'available'
                ? 'bg-blue-600 text-white'
                : 'bg-white text-gray-700 border border-gray-300 hover:bg-gray-50'
            }`}
          >
            Disponibles uniquement
          </button>
        </div>

        {/* Stats */}
        <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-green-100 p-2 rounded-lg">
                <CheckCircle2 className="w-5 h-5 text-green-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">
                  {chargingStations.filter(s => s.status === 'available').length}
                </p>
                <p className="text-sm text-gray-500">Bornes disponibles</p>
              </div>
            </div>
          </div>
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-red-100 p-2 rounded-lg">
                <Plug className="w-5 h-5 text-red-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">
                  {chargingStations.filter(s => s.status === 'occupied').length}
                </p>
                <p className="text-sm text-gray-500">Bornes occupées</p>
              </div>
            </div>
          </div>
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-blue-100 p-2 rounded-lg">
                <Battery className="w-5 h-5 text-blue-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">{chargingStations.length}</p>
                <p className="text-sm text-gray-500">Bornes totales</p>
              </div>
            </div>
          </div>
        </div>

        {/* Charging Stations List */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          {filteredStations.map((station) => (
            <button
              key={station.id}
              onClick={() => handleSelectStation(station.id)}
              disabled={station.status !== 'available'}
              className={`bg-white rounded-xl p-6 shadow-sm border-2 transition-all text-left ${
                selectedStation === station.id
                  ? 'border-blue-600 shadow-lg'
                  : station.status === 'available'
                  ? 'border-gray-200 hover:border-blue-300 hover:shadow-md'
                  : 'border-gray-200 opacity-60 cursor-not-allowed'
              }`}
            >
              <div className="flex items-start justify-between mb-4">
                <div className="flex items-center gap-3">
                  <div className={`p-3 rounded-lg ${
                    station.status === 'available' 
                      ? 'bg-green-100' 
                      : station.status === 'occupied'
                      ? 'bg-red-100'
                      : 'bg-yellow-100'
                  }`}>
                    <Plug className={`w-6 h-6 ${
                      station.status === 'available' 
                        ? 'text-green-600' 
                        : station.status === 'occupied'
                        ? 'text-red-600'
                        : 'text-yellow-600'
                    }`} />
                  </div>
                  <div>
                    <h3 className="font-bold text-gray-900">{station.name}</h3>
                    <div className="flex items-center gap-1 text-sm text-gray-500 mt-1">
                      <MapPin className="w-4 h-4" />
                      <span>{station.location}</span>
                    </div>
                  </div>
                </div>
                {selectedStation === station.id && (
                  <CheckCircle2 className="w-6 h-6 text-blue-600" />
                )}
              </div>

              <div className="flex flex-wrap gap-2 mb-3">
                <span className={`px-3 py-1 rounded-full text-xs font-medium border ${getStatusColor(station.status)}`}>
                  {getStatusText(station.status)}
                </span>
                <span className="px-3 py-1 rounded-full text-xs font-medium bg-blue-100 text-blue-700 border border-blue-200">
                  <Zap className="w-3 h-3 inline mr-1" />
                  {station.power}
                </span>
                <span className="px-3 py-1 rounded-full text-xs font-medium bg-purple-100 text-purple-700 border border-purple-200">
                  {station.type}
                </span>
              </div>

              <div className="text-sm text-gray-500">
                Distance: {station.distance}
              </div>
            </button>
          ))}
        </div>

        {/* Action Button */}
        {selectedStation && (
          <div className="fixed bottom-0 left-0 right-0 bg-white border-t border-gray-200 p-4 shadow-lg">
            <div className="max-w-7xl mx-auto flex gap-4">
              <button
                onClick={handleStartCharging}
                className="flex-1 bg-green-600 text-white py-4 rounded-xl font-medium hover:bg-green-700 transition-colors shadow-lg"
              >
                Démarrer la recharge
              </button>
            </div>
          </div>
        )}
      </main>

      {/* Add Station Modal */}
      {showAddStation && (
        <div className="fixed inset-0 bg-black/50 flex items-center justify-center z-50 p-4">
          <div className="bg-white rounded-2xl p-6 max-w-lg w-full max-h-[90vh] overflow-y-auto">
            <div className="flex items-center justify-between mb-6">
              <h3 className="text-2xl font-bold text-gray-900">Ajouter une borne</h3>
              <button
                onClick={() => setShowAddStation(false)}
                className="p-2 hover:bg-gray-100 rounded-lg transition-colors"
              >
                <X className="w-5 h-5 text-gray-500" />
              </button>
            </div>
            
            <form onSubmit={handleAddStation} className="space-y-4">
              <div>
                <label htmlFor="name" className="block text-sm font-medium text-gray-700 mb-1">
                  Nom de la borne *
                </label>
                <input
                  type="text"
                  id="name"
                  name="name"
                  value={formData.name}
                  onChange={handleInputChange}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                  placeholder="Borne G - Parking Nord"
                  required
                />
              </div>

              <div>
                <label htmlFor="location" className="block text-sm font-medium text-gray-700 mb-1">
                  Emplacement *
                </label>
                <input
                  type="text"
                  id="location"
                  name="location"
                  value={formData.location}
                  onChange={handleInputChange}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                  placeholder="Niveau 1, Zone D"
                  required
                />
              </div>

              <div>
                <label htmlFor="status" className="block text-sm font-medium text-gray-700 mb-1">
                  Statut *
                </label>
                <select
                  id="status"
                  name="status"
                  value={formData.status}
                  onChange={handleInputChange}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                  required
                >
                  <option value="available">Disponible</option>
                  <option value="occupied">Occupée</option>
                  <option value="maintenance">Maintenance</option>
                </select>
              </div>

              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label htmlFor="power" className="block text-sm font-medium text-gray-700 mb-1">
                    Puissance *
                  </label>
                  <input
                    type="text"
                    id="power"
                    name="power"
                    value={formData.power}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                    placeholder="22 kW"
                    required
                  />
                </div>

                <div>
                  <label htmlFor="type" className="block text-sm font-medium text-gray-700 mb-1">
                    Type de connecteur *
                  </label>
                  <input
                    type="text"
                    id="type"
                    name="type"
                    value={formData.type}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                    placeholder="Type 2, CCS..."
                    required
                  />
                </div>
              </div>

              <div>
                <label htmlFor="distance" className="block text-sm font-medium text-gray-700 mb-1">
                  Distance *
                </label>
                <input
                  type="text"
                  id="distance"
                  name="distance"
                  value={formData.distance}
                  onChange={handleInputChange}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                  placeholder="100m"
                  required
                />
              </div>

              <div className="flex gap-3 pt-4">
                <button
                  type="button"
                  onClick={() => setShowAddStation(false)}
                  className="flex-1 px-4 py-3 border border-gray-300 text-gray-700 rounded-lg hover:bg-gray-50 transition-colors font-medium"
                >
                  Annuler
                </button>
                <button
                  type="submit"
                  className="flex-1 px-4 py-3 bg-blue-600 text-white rounded-lg hover:bg-blue-700 transition-colors font-medium"
                >
                  Ajouter la borne
                </button>
              </div>
            </form>
          </div>
        </div>
      )}
    </div>
  );
}