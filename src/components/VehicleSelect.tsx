import { useState } from 'react';
import { ArrowLeft, Car, Plus, Battery, Zap, Calendar, CheckCircle2, X } from 'lucide-react';

interface VehicleSelectProps {
  onBack: () => void;
}

interface Vehicle {
  id: string;
  brand: string;
  model: string;
  year: number;
  battery: string;
  range: string;
  chargeLevel: number;
  lastCharge: string;
  image: string;
}

const initialVehicles: Vehicle[] = [
  {
    id: '1',
    brand: 'Tesla',
    model: 'Model 3',
    year: 2023,
    battery: '75 kWh',
    range: '580 km',
    chargeLevel: 85,
    lastCharge: 'Il y a 2 heures',
    image: '🚗'
  },
  {
    id: '2',
    brand: 'Renault',
    model: 'Zoe',
    year: 2022,
    battery: '52 kWh',
    range: '395 km',
    chargeLevel: 42,
    lastCharge: 'Hier',
    image: '🚙'
  },
  {
    id: '3',
    brand: 'Volkswagen',
    model: 'ID.4',
    year: 2024,
    battery: '77 kWh',
    range: '520 km',
    chargeLevel: 68,
    lastCharge: 'Il y a 5 heures',
    image: '🚕'
  }
];

const vehicleEmojis = ['🚗', '🚙', '🚕', '🚐', '🚘'];

export function VehicleSelect({ onBack }: VehicleSelectProps) {
  const [vehicles, setVehicles] = useState<Vehicle[]>(initialVehicles);
  const [selectedVehicle, setSelectedVehicle] = useState<string | null>(null);
  const [showAddVehicle, setShowAddVehicle] = useState(false);
  
  // Form states
  const [formData, setFormData] = useState({
    brand: '',
    model: '',
    year: new Date().getFullYear(),
    battery: '',
    range: '',
    chargeLevel: 50
  });

  const handleSelectVehicle = (vehicleId: string) => {
    setSelectedVehicle(vehicleId);
  };

  const handleAddVehicle = (e: React.FormEvent) => {
    e.preventDefault();
    
    const newVehicle: Vehicle = {
      id: Date.now().toString(),
      brand: formData.brand,
      model: formData.model,
      year: formData.year,
      battery: formData.battery,
      range: formData.range,
      chargeLevel: formData.chargeLevel,
      lastCharge: 'Jamais',
      image: vehicleEmojis[Math.floor(Math.random() * vehicleEmojis.length)]
    };

    setVehicles([...vehicles, newVehicle]);
    setShowAddVehicle(false);
    
    // Reset form
    setFormData({
      brand: '',
      model: '',
      year: new Date().getFullYear(),
      battery: '',
      range: '',
      chargeLevel: 50
    });
    
    alert(`Véhicule ${formData.brand} ${formData.model} ajouté avec succès!`);
  };

  const handleInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: name === 'year' || name === 'chargeLevel' ? parseInt(value) || 0 : value
    }));
  };

  const getChargeLevelColor = (level: number) => {
    if (level >= 70) return 'text-green-600 bg-green-100';
    if (level >= 30) return 'text-yellow-600 bg-yellow-100';
    return 'text-red-600 bg-red-100';
  };

  const getChargeLevelBarColor = (level: number) => {
    if (level >= 70) return 'bg-green-600';
    if (level >= 30) return 'bg-yellow-600';
    return 'bg-red-600';
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
                <h1 className="text-2xl font-bold text-gray-900">Mes Véhicules</h1>
                <p className="text-sm text-gray-500">Gérez vos véhicules électriques</p>
              </div>
            </div>
            <button
              onClick={() => setShowAddVehicle(true)}
              className="flex items-center gap-2 px-4 py-2 bg-purple-600 text-white rounded-lg hover:bg-purple-700 transition-colors"
            >
              <Plus className="w-5 h-5" />
              <span className="hidden sm:inline">Ajouter un véhicule</span>
            </button>
          </div>
        </div>
      </header>

      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        {/* Stats */}
        <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-purple-100 p-2 rounded-lg">
                <Car className="w-5 h-5 text-purple-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">{vehicles.length}</p>
                <p className="text-sm text-gray-500">Véhicules enregistrés</p>
              </div>
            </div>
          </div>
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-green-100 p-2 rounded-lg">
                <Battery className="w-5 h-5 text-green-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">
                  {Math.round(vehicles.reduce((sum, v) => sum + v.chargeLevel, 0) / vehicles.length)}%
                </p>
                <p className="text-sm text-gray-500">Charge moyenne</p>
              </div>
            </div>
          </div>
          <div className="bg-white rounded-lg p-4 shadow-sm border border-gray-200">
            <div className="flex items-center gap-3">
              <div className="bg-blue-100 p-2 rounded-lg">
                <Zap className="w-5 h-5 text-blue-600" />
              </div>
              <div>
                <p className="text-2xl font-bold text-gray-900">
                  {vehicles.reduce((sum, v) => sum + parseInt(v.battery), 0)} kWh
                </p>
                <p className="text-sm text-gray-500">Capacité totale</p>
              </div>
            </div>
          </div>
        </div>

        {/* Vehicles Grid */}
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {vehicles.map((vehicle) => (
            <button
              key={vehicle.id}
              onClick={() => handleSelectVehicle(vehicle.id)}
              className={`bg-white rounded-xl p-6 shadow-sm border-2 transition-all text-left hover:shadow-md ${
                selectedVehicle === vehicle.id
                  ? 'border-purple-600 shadow-lg'
                  : 'border-gray-200 hover:border-purple-300'
              }`}
            >
              {/* Vehicle Icon */}
              <div className="flex items-start justify-between mb-4">
                <div className="text-5xl mb-2">{vehicle.image}</div>
                {selectedVehicle === vehicle.id && (
                  <CheckCircle2 className="w-6 h-6 text-purple-600" />
                )}
              </div>

              {/* Vehicle Info */}
              <h3 className="text-xl font-bold text-gray-900 mb-1">
                {vehicle.brand} {vehicle.model}
              </h3>
              <p className="text-sm text-gray-500 mb-4">Année {vehicle.year}</p>

              {/* Battery Level */}
              <div className="mb-4">
                <div className="flex items-center justify-between mb-2">
                  <span className="text-sm font-medium text-gray-700">Niveau de charge</span>
                  <span className={`text-sm font-bold px-2 py-1 rounded ${getChargeLevelColor(vehicle.chargeLevel)}`}>
                    {vehicle.chargeLevel}%
                  </span>
                </div>
                <div className="w-full bg-gray-200 rounded-full h-2">
                  <div
                    className={`h-2 rounded-full transition-all ${getChargeLevelBarColor(vehicle.chargeLevel)}`}
                    style={{ width: `${vehicle.chargeLevel}%` }}
                  />
                </div>
              </div>

              {/* Specs */}
              <div className="space-y-2 border-t border-gray-200 pt-4">
                <div className="flex items-center justify-between text-sm">
                  <span className="text-gray-500">Batterie</span>
                  <span className="font-medium text-gray-900">{vehicle.battery}</span>
                </div>
                <div className="flex items-center justify-between text-sm">
                  <span className="text-gray-500">Autonomie</span>
                  <span className="font-medium text-gray-900">{vehicle.range}</span>
                </div>
                <div className="flex items-center gap-2 text-sm text-gray-500 mt-3">
                  <Calendar className="w-4 h-4" />
                  <span>Dernière recharge: {vehicle.lastCharge}</span>
                </div>
              </div>
            </button>
          ))}

          {/* Add Vehicle Card */}
          <button
            onClick={() => setShowAddVehicle(true)}
            className="bg-white rounded-xl p-6 border-2 border-dashed border-gray-300 hover:border-purple-400 transition-all flex flex-col items-center justify-center min-h-[300px] group"
          >
            <div className="bg-purple-100 p-4 rounded-full mb-4 group-hover:scale-110 transition-transform">
              <Plus className="w-8 h-8 text-purple-600" />
            </div>
            <h3 className="font-bold text-gray-900 mb-1">Ajouter un véhicule</h3>
            <p className="text-sm text-gray-500 text-center">
              Enregistrez un nouveau véhicule électrique
            </p>
          </button>
        </div>

        {/* Selected Vehicle Action */}
        {selectedVehicle && (
          <div className="fixed bottom-0 left-0 right-0 bg-white border-t border-gray-200 p-4 shadow-lg">
            <div className="max-w-7xl mx-auto">
              <button
                onClick={() => {
                  const vehicle = vehicles.find(v => v.id === selectedVehicle);
                  alert(`Véhicule sélectionné: ${vehicle?.brand} ${vehicle?.model}`);
                }}
                className="w-full bg-purple-600 text-white py-4 rounded-xl font-medium hover:bg-purple-700 transition-colors shadow-lg"
              >
                Utiliser ce véhicule
              </button>
            </div>
          </div>
        )}
      </main>

      {/* Add Vehicle Modal (Simple placeholder) */}
      {showAddVehicle && (
        <div className="fixed inset-0 bg-black/50 flex items-center justify-center z-50 p-4">
          <div className="bg-white rounded-2xl p-6 max-w-lg w-full max-h-[90vh] overflow-y-auto">
            <div className="flex items-center justify-between mb-6">
              <h3 className="text-2xl font-bold text-gray-900">Ajouter un véhicule</h3>
              <button
                onClick={() => setShowAddVehicle(false)}
                className="p-2 hover:bg-gray-100 rounded-lg transition-colors"
              >
                <X className="w-5 h-5 text-gray-500" />
              </button>
            </div>
            
            <form onSubmit={handleAddVehicle} className="space-y-4">
              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label htmlFor="brand" className="block text-sm font-medium text-gray-700 mb-1">
                    Marque *
                  </label>
                  <input
                    type="text"
                    id="brand"
                    name="brand"
                    value={formData.brand}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500"
                    placeholder="Tesla, Renault..."
                    required
                  />
                </div>

                <div>
                  <label htmlFor="model" className="block text-sm font-medium text-gray-700 mb-1">
                    Modèle *
                  </label>
                  <input
                    type="text"
                    id="model"
                    name="model"
                    value={formData.model}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500"
                    placeholder="Model 3, Zoe..."
                    required
                  />
                </div>
              </div>

              <div>
                <label htmlFor="year" className="block text-sm font-medium text-gray-700 mb-1">
                  Année *
                </label>
                <input
                  type="number"
                  id="year"
                  name="year"
                  value={formData.year}
                  onChange={handleInputChange}
                  min="2010"
                  max={new Date().getFullYear() + 1}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500"
                  required
                />
              </div>

              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label htmlFor="battery" className="block text-sm font-medium text-gray-700 mb-1">
                    Capacité batterie *
                  </label>
                  <input
                    type="text"
                    id="battery"
                    name="battery"
                    value={formData.battery}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500"
                    placeholder="75 kWh"
                    required
                  />
                </div>

                <div>
                  <label htmlFor="range" className="block text-sm font-medium text-gray-700 mb-1">
                    Autonomie *
                  </label>
                  <input
                    type="text"
                    id="range"
                    name="range"
                    value={formData.range}
                    onChange={handleInputChange}
                    className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500"
                    placeholder="580 km"
                    required
                  />
                </div>
              </div>

              <div>
                <label htmlFor="chargeLevel" className="block text-sm font-medium text-gray-700 mb-1">
                  Niveau de charge initial: {formData.chargeLevel}%
                </label>
                <input
                  type="range"
                  id="chargeLevel"
                  name="chargeLevel"
                  value={formData.chargeLevel}
                  onChange={handleInputChange}
                  min="0"
                  max="100"
                  className="w-full"
                />
                <div className="flex justify-between text-xs text-gray-500 mt-1">
                  <span>0%</span>
                  <span>50%</span>
                  <span>100%</span>
                </div>
              </div>

              <div className="flex gap-3 pt-4">
                <button
                  type="button"
                  onClick={() => setShowAddVehicle(false)}
                  className="flex-1 px-4 py-3 border border-gray-300 text-gray-700 rounded-lg hover:bg-gray-50 transition-colors font-medium"
                >
                  Annuler
                </button>
                <button
                  type="submit"
                  className="flex-1 px-4 py-3 bg-purple-600 text-white rounded-lg hover:bg-purple-700 transition-colors font-medium"
                >
                  Ajouter le véhicule
                </button>
              </div>
            </form>
          </div>
        </div>
      )}
    </div>
  );
}